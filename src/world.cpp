#include "config.h"
#include "epithelial_cell.h"
#include "immune_cell.h"
#include "utils.h"
#include "window.h"
#include "world.h"

#include <math.h>
#include <utility>
#include <queue>

World::World(FILE* out_fp, FILE* section_fp) 
    : n_cells_in_section(kGridHeight / kNumSections)
    , timestep(0)
    , base_recruitment_immune_cells(floor(kRecruitment))
    , recruitment_probability(kRecruitment - base_recruitment_immune_cells)
    , x_distribution(0, kGridWidth - 1) 
    , y_distribution(0, kGridHeight - 1) 
    , epithelial_age_distribution(0, kCellLifespan) 
    , immune_age_distribution(0, kImmLifespan) 
    , division_time_distribution(0, kDivisionTime) {

    // initialise counts structure
    for (int i = 0; i < kNumSections; i++) {
        prev_counts.push_back({0, 0, 0, 0, 0, 0});
        counts.push_back({0, 0, 0, 0, 0, 0});
    }

    // initialise healthy cells
    printf("Initialising healthy cells\n");
    epithelial_cells = new EpithelialCell**[kGridWidth];
    for (int x = 0; x < kGridWidth; x++) {
        epithelial_cells[x] = new EpithelialCell*[kGridHeight];

        for (int y = 0; y < kGridHeight; y++) {
            int age = RandomEpithelialAge();
            int division_time = RandomDivisionTime();
            epithelial_cells[x][y] = new EpithelialCell(x, y, age, 0, division_time);
        }
    }

    // TODO: Convert to depth in cm rather than depth to cell
    int min_cell_depth = 0;
    int max_cell_depth = kStvInfectInitLoc;

    // initialise infected cells
    printf("Initialising infected cells\n"); 
    int initial_infected = kTotalEpithelialCells * kStvInfectInit;
    for (int i = 0; i < initial_infected; i++) {
        int x = RandomX();
        int y = RandomY();

        if (epithelial_cells[x][y]->state == EpithelialState::HEALTHY) {
            // initialise at a specific depth and with a spread
            if (y >= min_cell_depth && y <= max_cell_depth) {
                int section = (int)floor(y / n_cells_in_section);
                counts[section].s_infected++;
                epithelial_cells[x][y]->state = EpithelialState::S_INFECTED;
            }
            else {
                i--;
            }
        }
        else {
            i--;
        }
    }

    // initialise dead cells (should be 0 most of the time)
    printf("Initialising dead cells\n");
    int initial_dead = kTotalEpithelialCells * kDeadInit;
    for (int i = 0; i < initial_dead; i++) {
        int x = RandomX();
        int y = RandomY();

        if (epithelial_cells[x][y]->state == EpithelialState::HEALTHY) {
            int section = (int)floor(y / n_cells_in_section);
            counts[section].dead++;
            epithelial_cells[x][y]->state = EpithelialState::DEAD;
        }
        else {
            i--;
        }
    }

    // set the counts for healthy
    int epithelial_cells_per_section = kTotalEpithelialCells / kNumSections;
    for (int i = 0; i < kNumSections; i++) {
        counts[i].healthy = epithelial_cells_per_section - counts[i].dead - counts[i].s_infected;
    }

    // initialise immune cells
    for (int i = 0; i < kBaseImmCell; i++) {
        int x = RandomX();
        int y = RandomY();
        int age = RandomImmuneAge();
        ImmuneCell immune_cell = ImmuneCell(x, y, age, ImmuneState::VIRGIN);
        immune_cells.push_back(immune_cell);

        int section = (int) floor(y / kNumSections);
        counts[section].immune++;
    }

    // print the initial conditions to file
    PrintTimeStepToFile(out_fp, section_fp);
}

World::~World() {
    // clear
    for (int x = 0; x < kGridWidth; x++) {
        for (int y = 0; y < kGridHeight; y++) {
            delete epithelial_cells[x][y];
        }

        delete epithelial_cells[x];
    }

    delete epithelial_cells;
}

void World::Step(FILE* out_fp, FILE* section_fp) {
    // add if time for introduction fo extra DIPs
    if (timestep == kDipExtTime) {
        AddExtDip();
    }

    // add to the timestep
    timestep++;

    // flip counts
    std::copy(counts.begin(), counts.end(), prev_counts.begin());
    for (int i = 0; i < kNumSections; i++) {
        counts[i] = {0, 0, 0, 0, 0, 0};
    }

    // epithelial cells
    UpdateEpithelialCells();

    // immune cells
    UpdateImmuneCells();

    // print the counts
    PrintTimeStepToFile(out_fp, section_fp);
}

void World::UpdateEpithelialCells() {
    // assign previous states
    for (int x = 0; x < kGridWidth; x++) {
        for (int y = 0; y < kGridHeight; y++) {
            epithelial_cells[x][y]->FlipStates();
        }
    }

    // update states
    for (int x = 0; x < kGridWidth; x++) {
        for (int y = 0; y < kGridHeight; y++) {
            epithelial_cells[x][y]->Update(*this);

            int section = (int) floor(y / n_cells_in_section);

            // update counts
            EpithelialState state = epithelial_cells[x][y]->state;
            if (state == EpithelialState::HEALTHY) {
                counts[section].healthy++;
            }
            else if (state == EpithelialState::DEAD) {
                counts[section].dead++;
            }
            else if (state == EpithelialState::S_INFECTED ||
                     state == EpithelialState::S_EXPRESSING || 
                     state == EpithelialState::S_INFECTIOUS) {
                counts[section].s_infected++;
            }
            else if (state == EpithelialState::D_INFECTED) {
                counts[section].d_infected++;
            }
            else if (state == EpithelialState::C_INFECTED || 
                     state == EpithelialState::C_EXPRESSING || 
                     state == EpithelialState::C_INFECTIOUS) {
                counts[section].c_infected++;
            }
        }
    }
}

void World::UpdateImmuneCells() {
    int n_virgin = 0;
    // update first and remove if already dead
    for (auto cell = immune_cells.begin(); cell != immune_cells.end();) {
        (*cell).Update(*this);

        // update count if it is active
        if ((*cell).state == ImmuneState::DEAD) {
            cell = immune_cells.erase(cell);
        }
        else {
            if ((*cell).age >= 0) {
                int section = (int) floor((*cell).y / n_cells_in_section);
                counts[section].immune++;
            }

            // count virgin cells to maintain base immune cells
            if ((*cell).state == ImmuneState::VIRGIN) {
                n_virgin++;
            }
        
            cell++;
        }
    }

    // maintain base imm virgin cells
    for (; n_virgin < kBaseImmCell; n_virgin++) {
        int x = RandomX();
        int y = RandomY();
        int section = (int) floor(y / n_cells_in_section);
        immune_cells.push_back(ImmuneCell(x, y, 0, ImmuneState::VIRGIN));
        counts[section].immune++;
    }

    // add newly recruited mature immune cells (transfer from new_mature_immune_cells to
    // immune_cells)
    std::move(new_mature_immune_cells.begin(), new_mature_immune_cells.end(), std::back_inserter(immune_cells));
    new_mature_immune_cells.erase(new_mature_immune_cells.begin(), new_mature_immune_cells.end());
}

void World::MatureImmuneCellRecognitionEvent(int x, int y) {
    EpithelialState epithelial_state = epithelial_cells[x][y]->state;
    int section = (int)floor(y / n_cells_in_section);
    if (epithelial_state != EpithelialState::DEAD) {
        counts[section].dead++;

        if (epithelial_state == EpithelialState::S_EXPRESSING || 
            epithelial_state == EpithelialState::S_INFECTIOUS) {
            counts[section].s_infected--;
        }
        else if (epithelial_state == EpithelialState::D_INFECTED) {
            counts[section].d_infected--;
        }
        else {
            counts[section].c_infected--;
        }
        epithelial_cells[x][y]->state = EpithelialState::DEAD;
    }

    for (int i = 0; i < base_recruitment_immune_cells; i++) {
        AddNewImmuneCellFromRecognition(x, y);
    }

    // see if random recruitment is added
    if (random_p() < recruitment_probability) {
        AddNewImmuneCellFromRecognition(x, y);
    }
}

void World::AddNewImmuneCellFromRecognition(int x, int y) {
    if (kRandomImmuneCellSpawn) {
        int random_x = RandomX();
        int random_y = RandomY();
        new_mature_immune_cells.push_back(ImmuneCell(random_x, random_y, -kRecruitDelay, ImmuneState::MATURE));
    }
    else {
        new_mature_immune_cells.push_back(ImmuneCell(x, y, -kRecruitDelay, ImmuneState::MATURE));
    }
}

void World::PrintTimeStepToFile(FILE* out_fp, FILE* section_fp) {
    double p_healthy = 0;
    double p_stv_infected = 0;
    double p_dip_infected = 0;
    double p_co_infected = 0;
    double p_dead = 0;
    double p_immune = 0;

    // print for section cell counts 
    for (int i = 0; i < kNumSections; i++) {
        p_healthy += counts[i].healthy;
        p_stv_infected += counts[i].s_infected;
        p_dip_infected += counts[i].d_infected;
        p_co_infected += counts[i].c_infected;
        p_dead += counts[i].dead;
        p_immune += counts[i].immune;

        double ps_healthy = 1.0 * counts[i].healthy / n_cells_in_section;
        double ps_stv_infected = 1.0 * counts[i].s_infected / n_cells_in_section;
        double ps_dip_infected = 1.0 * counts[i].d_infected / n_cells_in_section;
        double ps_co_infected = 1.0 * counts[i].c_infected / n_cells_in_section;
        double ps_dead = 1.0 * counts[i].dead / n_cells_in_section;
        double ps_immune = 1.0 * counts[i].immune / n_cells_in_section;

        fprintf(section_fp, "%f,%f,%f,%f,%f,%f", ps_healthy, ps_stv_infected, ps_dip_infected, ps_co_infected, ps_dead, ps_immune);

        if (i < kNumSections - 1) {
            fprintf(section_fp, ":");
        }
    }
    fprintf(section_fp, "\n");

    // print for total cell counts
    p_healthy *= 1.0 / kTotalEpithelialCells;
    p_stv_infected *= 1.0 / kTotalEpithelialCells;
    p_dip_infected *= 1.0 / kTotalEpithelialCells;
    p_co_infected *= 1.0 / kTotalEpithelialCells;
    p_dead *= 1.0 / kTotalEpithelialCells;
    p_immune *= 1.0 / kTotalEpithelialCells;

    fprintf(out_fp, "%f,%f,%f,%f,%f,%f\n", p_healthy, p_stv_infected, p_dip_infected, p_co_infected, p_dead, p_immune);
}

void World::AddExtDip() {
    int dip_count = kTotalEpithelialCells * kDipExtInit;

    // TODO: change to cm
    int min_cell_depth = 0;
    int max_cell_depth = kDipExtLoc;

    for (int i = 0; i < dip_count; i++) {
        int x = RandomX();
        int y = RandomY();

        if (y < min_cell_depth || y > max_cell_depth) {
            i--;
            continue;
        }

        EpithelialState state = epithelial_cells[x][y]->state;
        
        // change the state (only needed for healthy and stv-infected cells)
        if (state == EpithelialState::HEALTHY) {
            epithelial_cells[x][y]->state = EpithelialState::D_INFECTED;
        }
        else if (state == EpithelialState::S_INFECTED) {
            epithelial_cells[x][y]->state = EpithelialState::C_INFECTED;
        }
    }
}

void World::PrintInfo(FILE *fp) {
    // print proportions
    PrintProportionHealthyOfDeadNeighbours(fp);
    fprintf(fp, "+");

    // print healthy patch sizes
    PrintPatchSizes(fp, EpithelialState::HEALTHY);
    fprintf(fp, "+");

    // print dead patch sizes
    PrintPatchSizes(fp, EpithelialState::DEAD);
}

void World::PrintPatchSizes(FILE* fp, EpithelialState patch_state) {
    std::vector<int> patch_sizes = GetPatchSizes(patch_state);

    int size = patch_sizes.size();
    for (int i = 0; i < size; i++) {
        fprintf(fp, "%d", patch_sizes[i]);

        if (i != size - 1) {
            fprintf(fp, ",");
        }
    }
}

std::vector<int> World::GetPatchSizes(EpithelialState patch_state) {
    std::vector<std::vector<int>> visited(kGridWidth, std::vector<int>(kGridHeight, 0));

    std::vector<int> sizes;

    int dx[2] = {-1, 1};
    int dy[2] = {-1, 1};
    
    for (int x = 0; x < kGridWidth; x++) {
        for (int y = 0; y < kGridHeight; y++) {
            // do a bfs for the current cell if it hasn't been visited 
            if (!visited[x][y] && epithelial_cells[x][y]->state == patch_state) {
                int n_dead_cells = 0;

                std::queue<std::pair<int, int>> q;

                q.push(std::pair<int, int> (x, y));
                visited[x][y] = 1;

                std::pair<int, int> curr;
                while (!q.empty()) {
                    curr = q.front();

                    // if not a dead epithelial cell then just continue
                    if (epithelial_cells[curr.first][curr.second]->state != patch_state) {
                        q.pop();
                        continue;
                    }

                    // add to the counter of dead cells since the current epithelial cell is dead
                    n_dead_cells++;

                    // add north, south, east and west neighbours
                    for (int i = 0; i < 2; i++) {
                        int new_x = return_in_bounds_x(curr.first + dx[i]);
                        int new_y = return_in_bounds_y(curr.second + dy[i]);

                        if (is_out_of_bounds_y(new_y)) continue;

                        if (!visited[new_x][curr.second]) {
                            visited[new_x][curr.second] = 1;
                            q.push(std::pair(new_x, curr.second));
                        }

                        if (!visited[curr.first][new_y]) {
                            visited[curr.first][new_y] = 1;
                            q.push(std::pair(curr.first, new_y));
                        }
                    }

                    q.pop();
                }

                // append the patch size to the list
                sizes.push_back(n_dead_cells);
            }
            else {
                visited[x][y] = 1;
            }
        }
    }

    return sizes;
}

void World::PrintProportionHealthyOfDeadNeighbours(FILE* fp) {
    int n_dead = 0;
    int n_dead_with_healthy_neighbour = 0;

    int n_healthy;
    for (int x = 0; x < kGridWidth; x++) {
        for (int y = 0; y < kGridHeight; y++) {
            EpithelialState state = epithelial_cells[x][y]->state;
            if (state == EpithelialState::DEAD) {
                n_dead++;
                n_healthy = 0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int new_x = return_in_bounds_x(x + i);
                        int new_y = return_in_bounds_y(y + j);

                        if (is_out_of_bounds_y(new_y)) continue;

                        if (epithelial_cells[new_x][new_y]->state == EpithelialState::HEALTHY) {
                            n_healthy++;
                        }
                    }
                }

                // count only dead cells with healthy neighbours
                if (n_healthy > 0) {
                    n_dead_with_healthy_neighbour++;
                }
            }
        }
    }

    double average_healthy_per_dead = 1.0 * n_dead_with_healthy_neighbour / n_dead;

    fprintf(fp, "%f", average_healthy_per_dead);
}

int World::RandomX() {
    return x_distribution(rng);
}

int World::RandomY() {
    return y_distribution(rng);
}

int World::RandomEpithelialAge() {
    return epithelial_age_distribution(rng);
}

int World::RandomImmuneAge() {
    return immune_age_distribution(rng);
}

int World::RandomDivisionTime() {
    return division_time_distribution(rng);
}
