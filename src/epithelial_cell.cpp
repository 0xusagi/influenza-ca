#include "config.h"
#include "epithelial_cell.h"
#include "utils.h"
#include "world.h"

EpithelialCell::EpithelialCell(int x, int y, int age, int infect_time, int time_left_to_divide) :
    x(x),
    y(y),
    age(age),
    infect_time(infect_time),
    time_left_to_divide(time_left_to_divide),
    prev_state(EpithelialState::HEALTHY),
    state(EpithelialState::HEALTHY) {}

void EpithelialCell::Update(World& world) {
    age++;

    if (state == EpithelialState::HEALTHY) {
        UpdateHealthy(world);
    }
    else if (state == EpithelialState::S_INFECTED) {
        infect_time++;
        UpdateStvInfected(world);
    }
    else if (state == EpithelialState::S_EXPRESSING) {
        infect_time++;
        UpdateStvExpressing();
    }
    else if (state == EpithelialState::S_INFECTIOUS) {
        infect_time++;
        UpdateStvInfectious();
    }
    else if (state == EpithelialState::D_INFECTED) {
        UpdateDipInfected(world);
    }
    else if (state == EpithelialState::C_INFECTED) {
        infect_time++;
        UpdateCoInfected();
    }
    else if (state == EpithelialState::C_EXPRESSING) {
        infect_time++;
        UpdateCoExpressing();
    }
    else if (state == EpithelialState::C_INFECTIOUS) {
        infect_time++;
        UpdateCoInfectious();
    }
    else {
        UpdateDead(world);
    }

    // cell has already divided 
    // renew division time
    time_left_to_divide--;
    if (time_left_to_divide < 0) {
        time_left_to_divide = kDivisionTime;
    }
}

void EpithelialCell::FlipStates() {
    prev_state = state;
}

void EpithelialCell::UpdateHealthy(World& world) {
    if (IsDeadFromOldAge()) {
        state = EpithelialState::DEAD;
        return;
    }

    // healthy cell becomes infected
    int stv = 0;
    int dip = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int new_x = return_in_bounds_x(x + i);
            int new_y = return_in_bounds_y(y + j);

            // boundary condition for y
            if (is_out_of_bounds_y(new_y)) continue;

            EpithelialState neighbour_state = world.epithelial_cells[new_x][new_y]->prev_state;

            double multiplier = neighbour_infect_multiplier[i + 1][j + 1];
            
            // neighbour with stv only
            if (neighbour_state == EpithelialState::S_INFECTIOUS) {
                // cell gets infected
                if (random_p() < multiplier * kStvInfectRate / 8) {
                    // determine whether stv or dip infected the cell
                    if (random_p() < kStvErrorRate) {
                        dip = 1;
                    }
                    else {
                        stv = 1;
                    }
                }
            }

            // neighbour with both stv and dip
            else if (neighbour_state == EpithelialState::C_INFECTIOUS) {
                // cell gets infected
                if (random_p() < multiplier * kCoInfectRate / 8) {
                    // determine whether stv or dip infected the cell
                    if (random_p() < kCoErrorRate) {
                        stv = 1;
                    } 
                    else {
                        dip = 1;
                    }
                }
            }
        }
    }

    // assign the state to the epithelial cell
    if (stv && dip) {
        state = EpithelialState::C_INFECTED;
        infect_time = 0;
    } 
    else if (stv) {
        state = EpithelialState::S_INFECTED;
        infect_time = 0;
    }
    else if (dip) {
        state = EpithelialState::D_INFECTED;
        infect_time = 0;
    }
}

void EpithelialCell::UpdateStvInfected(World& world) {
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }

    // becomes expressing
    if (infect_time > kStvExpressDelay) {
        state = EpithelialState::S_EXPRESSING;
        return;
    }

    // check if receives a dip 
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int new_x = return_in_bounds_x(x + i);
            int new_y = return_in_bounds_y(y + j);

            // boundary condition for y
            if (is_out_of_bounds_y(new_y)) continue;

            EpithelialState neighbour_state = world.epithelial_cells[new_x][new_y]->prev_state;

            double multiplier = neighbour_infect_multiplier[i + 1][j + 1];

            // from a stv-infected neighbour 
            if (neighbour_state == EpithelialState::S_INFECTIOUS) {
                if (random_p() < multiplier * kStvInfectRate / 8) {
                    if (random_p() < kStvErrorRate) {
                        state = EpithelialState::C_INFECTIOUS;
                        return;
                    }
                }
            }
            // from a co-infected neighbour
            else if (neighbour_state == EpithelialState::C_INFECTIOUS) {
                if (random_p() < multiplier * kCoInfectRate / 8) {
                    // only consider the rate at which receives a dip since already 
                    // infected with stv
                    if (random_p() >= kCoErrorRate) {
                        // TODO: Is this needed to reset the timer?
                        state = EpithelialState::C_INFECTIOUS;
                        return;
                    }
                }
            }
        }
    }
}

void EpithelialCell::UpdateStvExpressing() {
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }

    // becomes infectious
    if (infect_time > kStvInfectDelay) {
        state = EpithelialState::S_INFECTIOUS;
        return;
    }
}

void EpithelialCell::UpdateStvInfectious() {
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }
}

void EpithelialCell::UpdateDipInfected(World& world) {
    if (IsDeadFromOldAge()) {
        state = EpithelialState::DEAD;
        return;
    }

    // check if receives a stv 
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int new_x = return_in_bounds_x(x + i);
            int new_y = return_in_bounds_y(y + j);

            // boundary condition for y
            if (is_out_of_bounds_y(new_y)) continue;

            EpithelialState neighbour_state = world.epithelial_cells[new_x][new_y]->prev_state;

            double multiplier = neighbour_infect_multiplier[i + 1][j + 1];

            // from a stv-infected neighbour 
            if (neighbour_state == EpithelialState::S_INFECTIOUS) {
                if (random_p() < multiplier * kStvInfectRate / 8) {
                    if (random_p() >= kStvErrorRate) {
                        state = EpithelialState::C_INFECTIOUS;
                        infect_time = 0;
                        return;
                    }
                }
            }
            // from a co-infected neighbour
            else if (neighbour_state == EpithelialState::C_INFECTIOUS) {
                if (random_p() < multiplier * kCoInfectRate / 8) {
                    // only consider the rate at which receives a dip since already 
                    // infected with stv
                    if (random_p() < kCoErrorRate) {
                        // TODO: Is this needed to reset the timer?
                        state = EpithelialState::C_INFECTIOUS;
                        infect_time = 0;
                        return;
                    }
                }
            }
        }
    }

}

void EpithelialCell::UpdateCoInfected() {
    // is dead
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }

    // becomes expressing
    if (infect_time > kCoExpressDelay) {
        state = EpithelialState::C_EXPRESSING;
        return;
    }
}

void EpithelialCell::UpdateCoExpressing() {
    // is dead
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }

    // becomes infectious
    if (infect_time > kCoInfectDelay) {
        state = EpithelialState::C_INFECTIOUS;
        return;
    }
}

void EpithelialCell::UpdateCoInfectious() {
    // is dead
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }
}

void EpithelialCell::UpdateDead(World& world) {
    // revive cell
    double p_able_to_divide;
    if (kGlobalEpithelialDivision) {
        p_able_to_divide = GetGlobalDivisionRate(world);
    } 
    else {
        p_able_to_divide = GetLocalDivisionRate(world);
    }

    double p = random_p();
    if (p < p_able_to_divide) {
        state = EpithelialState::HEALTHY;
        age = 0;
        infect_time = 0;
    }
}

double EpithelialCell::GetGlobalDivisionRate(World& world) {
    int healthy_count = 0;
    int dead_count = 0;

    for (int i = 0; i < kNumSections; i++) {
        healthy_count += world.prev_counts[i].healthy;
        dead_count += world.prev_counts[i].dead;
    }

    return 1 / kDivisionTime * healthy_count / dead_count;
}

double EpithelialCell::GetLocalDivisionRate(World& world) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // skip current cell
            if (i == 0 && j == 0) continue;

            // if the neighbour is healthy and is time to divide,
            int new_x = return_in_bounds_x(x + i);
            int new_y = return_in_bounds_y(y + j);

            // boundary condition for y 
            if (is_out_of_bounds_y(new_y)) continue;

            EpithelialState neighbour_state = world.epithelial_cells[new_x][new_y]->state;
            if (neighbour_state == EpithelialState::HEALTHY && world.epithelial_cells[new_x][new_y]->time_left_to_divide == 0) {
                // return uniform probability for each neighbour
                world.epithelial_cells[new_x][new_y]->time_left_to_divide--;
                return 1.0;
            }
        }
    }

    return 0.0;
}

int EpithelialCell::IsDeadFromOldAge() {
    if (age > kCellLifespan) 
        return 1;

    return 0;
}

int EpithelialCell::IsDeadFromInfection() {
    if (infect_time > kInfectLifespan) 
        return 1;

    return 0;
}
