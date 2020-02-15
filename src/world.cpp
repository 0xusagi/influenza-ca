#include "config.h"
#include "epithelial_cell.h"
#include "utils.h"
#include "world.h"

World::World() 
    : prev_counts{0}
    , counts{0} 
    , x_distribution(0, kGridWidth - 1) 
    , y_distribution(0, kGridHeight - 1) 
    , epithelial_age_distribution(0, kCellLifespan) 
    , immune_age_distribution(0, kImmLifespan) {
    // initialise healthy cells
    printf("Initialising healthy cells\n");
    epithelial_cells = new EpithelialCell**[kGridWidth];
    for (int x = 0; x < kGridWidth; x++) {
        epithelial_cells[x] = new EpithelialCell*[kGridHeight];

        for (int y = 0; y < kGridHeight; y++) {
            int age = RandomEpithelialAge();
            epithelial_cells[x][y] = new EpithelialCell(x, y, age, 0, EpithelialState::HEALTHY);
        }
    }

    // initialise infected cells
    printf("Initialising infected cells\n");
    counts.infected = kTotalEpithelialCells * kInfectInit;
    for (int i = 0; i < counts.infected; i++) {
        int x = RandomX();
        int y = RandomY();

        if (epithelial_cells[x][y]->state == EpithelialState::HEALTHY) {
            epithelial_cells[x][y]->state = EpithelialState::INFECTED;
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
    counts.healthy = kTotalEpithelialCells - counts.infected - counts.dead;
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

void World::Simulate(FILE* fp) {
    // print the initial conditions to file
    PrintTimeStepToFile(fp);

    // start simulation
    for (int t = 0; t < kSimulationLength; t++) {
        // flip counts
        prev_counts = counts;
        counts = {0};

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
                else {
                    counts.infected++;
                }
            }
        }

        // print the counts
        PrintTimeStepToFile(fp);
    }
}

void World::PrintTimeStepToFile(FILE* fp) {
    double p_healthy = 1.0 * counts.healthy / kTotalEpithelialCells;
    double p_infected = 1.0 * counts.infected / kTotalEpithelialCells;
    double p_dead = 1.0 * counts.dead / kTotalEpithelialCells;
    double p_immune = 1.0 * counts.immune / kTotalEpithelialCells;

    fprintf(fp, "%f,%f,%f,%f\n", p_healthy, p_infected, p_dead, p_immune);
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