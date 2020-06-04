#include "config.h"
#include "epithelial_cell.h"
#include "immune_cell.h"
#include "utils.h"
#include "window.h"
#include "world.h"

#include <math.h>

World::World(FILE* fp) 
    : prev_counts{0}
    , counts{0} 
    , timestep(0)
    , base_recruitment_immune_cells(floor(kRecruitment))
    , recruitment_probability(kRecruitment - base_recruitment_immune_cells)
    , x_distribution(0, kGridWidth - 1) 
    , y_distribution(0, kGridHeight - 1) 
    , epithelial_age_distribution(0, kCellLifespan) 
    , immune_age_distribution(0, kImmLifespan) 
    , division_time_distribution(0, kDivisionTime) {
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

    // initialise infected cells
    printf("Initialising infected cells\n"); 
    counts.s_infected = kTotalEpithelialCells * kStvInfectInit;
    for (int i = 0; i < counts.s_infected; i++) {
        int x = RandomX();
        int y = RandomY();

        if (epithelial_cells[x][y]->state == EpithelialState::HEALTHY) {
            epithelial_cells[x][y]->state = EpithelialState::S_INFECTED;
        }
        else {
            i--;
        }
    }

    // initialise dead cells (should be 0 most of the time)
    printf("Initialising dead cells\n");
    counts.dead = kTotalEpithelialCells * kDeadInit;
    for (int i = 0; i < counts.dead; i++) {
        int x = RandomX();
        int y = RandomY();

        if (epithelial_cells[x][y]->state == EpithelialState::HEALTHY) {
            epithelial_cells[x][y]->state = EpithelialState::DEAD;
        }
        else {
            i--;
        }
    }

    // set the counts for healthy
    counts.healthy = kTotalEpithelialCells - counts.s_infected - counts.dead;

    // initialise immune cells
    for (int i = 0; i < kBaseImmCell; i++) {
        int x = RandomX();
        int y = RandomY();
        int age = RandomImmuneAge();
        ImmuneCell immune_cell = ImmuneCell(x, y, age, ImmuneState::VIRGIN);
        immune_cells.push_back(immune_cell);

        counts.immune++;
    }

    // print the initial conditions to file
    PrintTimeStepToFile(fp);
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

void World::Step(FILE* fp) {
    // add if time for introduction fo extra DIPs
    if (timestep == kDipExtTime) {
        AddExtDip();
    }

    // add to the timestep
    timestep++;

    // flip counts
    prev_counts = counts;
    counts = {0};

    // epithelial cells
    UpdateEpithelialCells();

    // immune cells
    UpdateImmuneCells();

    // print the counts
    PrintTimeStepToFile(fp);
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

            // update counts
            EpithelialState state = epithelial_cells[x][y]->state;
            if (state == EpithelialState::HEALTHY) {
                counts.healthy++;
            }
            else if (state == EpithelialState::DEAD) {
                counts.dead++;
            }
            else if (state == EpithelialState::S_INFECTED ||
                     state == EpithelialState::S_EXPRESSING || 
                     state == EpithelialState::S_INFECTIOUS) {
                counts.s_infected++;
            }
            else if (state == EpithelialState::D_INFECTED) {
                counts.d_infected++;
            }
            else if (state == EpithelialState::C_INFECTED || 
                     state == EpithelialState::C_EXPRESSING || 
                     state == EpithelialState::C_INFECTIOUS) {
                counts.c_infected++;
            }
        }
    }
}

void World::UpdateImmuneCells() {
    int n_virgin = 0;
    immune_cells_to_be_added = 0;
    // update first and remove if already dead
    for (auto cell = immune_cells.begin(); cell != immune_cells.end();) {
        (*cell).Update(*this);

        // update count if it is active
        if ((*cell).state == ImmuneState::DEAD) {
            cell = immune_cells.erase(cell);
        }
        else {
            if ((*cell).age >= 0) {
                counts.immune++;
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
        immune_cells.push_back(ImmuneCell(x, y, 0, ImmuneState::VIRGIN));
        counts.immune++;
    }

    // add newly recruited mature immune cells
    for (int i = 0; i < immune_cells_to_be_added; i++) {
        int x = RandomX();
        int y = RandomY();
        immune_cells.push_back(ImmuneCell(x, y, -kRecruitDelay, ImmuneState::MATURE));
    }
}

void World::MatureImmuneCellRecognitionEvent(int x, int y) {
    EpithelialState epithelial_state = epithelial_cells[x][y]->state;
    if (epithelial_state != EpithelialState::DEAD) {
        counts.dead++;

        if (epithelial_state == EpithelialState::S_EXPRESSING || 
            epithelial_state == EpithelialState::S_INFECTIOUS) {
            counts.s_infected--;
        }
        else if (epithelial_state == EpithelialState::D_INFECTED) {
            counts.d_infected--;
        }
        else {
            counts.c_infected--;
        }
        epithelial_cells[x][y]->state = EpithelialState::DEAD;
    }

    for (int i = 0; i < base_recruitment_immune_cells; i++) {
        immune_cells_to_be_added++;
    }

    // see if random recruitment is added
    if (random_p() < recruitment_probability) {
        immune_cells_to_be_added++;
    }
}

void World::PrintTimeStepToFile(FILE* fp) {
    double p_healthy = 1.0 * counts.healthy / kTotalEpithelialCells;
    double p_stv_infected = 1.0 * counts.s_infected / kTotalEpithelialCells;
    double p_dip_infected = 1.0 * counts.d_infected / kTotalEpithelialCells;
    double p_co_infected = 1.0 * counts.c_infected / kTotalEpithelialCells;
    double p_dead = 1.0 * counts.dead / kTotalEpithelialCells;
    double p_immune = 1.0 * counts.immune / kTotalEpithelialCells;

    fprintf(fp, "%f,%f,%f,%f,%f,%f\n", p_healthy, p_stv_infected, p_dip_infected, p_co_infected, p_dead, p_immune);
}

void World::AddExtDip() {
    int dip_count = kTotalEpithelialCells * kDipExtInit;

    for (int i = 0; i < dip_count; i++) {
        int x = RandomX();
        int y = RandomY();

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
