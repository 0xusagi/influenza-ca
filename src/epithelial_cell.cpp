#include "config.h"
#include "epithelial_cell.h"
#include "utils.h"
#include "world.h"

EpithelialCell::EpithelialCell(int x, int y, int age, int infect_time, int time_left_to_divide, EpithelialState state) :
    x(x),
    y(y),
    age(age),
    infect_time(infect_time),
    time_left_to_divide(time_left_to_divide),
    state(state) {}

void EpithelialCell::Update(World& world) {
    age++;
    time_left_to_divide--;

    // cell has already divided 
    // renew division time
    if (time_left_to_divide < 0) {
        time_left_to_divide = kDivisionTime;
    }

    if (state == EpithelialState::HEALTHY) {
        UpdateHealthy(world);
    }
    else if (state == EpithelialState::INFECTED) {
        infect_time++;
        UpdateInfected();
    }
    else if (state == EpithelialState::EXPRESSING) {
        infect_time++;
        UpdateExpressing();
    }
    else if (state == EpithelialState::INFECTIOUS) {
        infect_time++;
        UpdateInfectious();
    }
    else {
        UpdateDead(world);
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
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int new_x = return_in_bounds_x(x + i);
            int new_y = return_in_bounds_y(y + j);
            EpithelialState neighbour_state = world.epithelial_cells[new_x][new_y]->prev_state;
            if (neighbour_state == EpithelialState::INFECTIOUS) {
                if (random_p() < kInfectRate / 8) {
                    state = EpithelialState::INFECTED;
                    infect_time = 0;
                    return;
                }
            }
        }
    }
}

void EpithelialCell::UpdateInfected() {
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }

    // becomes expressing
    if (infect_time > kExpressDelay) {
        state = EpithelialState::EXPRESSING;
        return;
    }
}

void EpithelialCell::UpdateExpressing() {
    if (IsDeadFromOldAge() || IsDeadFromInfection()) {
        state = EpithelialState::DEAD;
        return;
    }

    // beocmes infectious
    if (infect_time > kInfectDelay) {
        state = EpithelialState::INFECTIOUS;
        return;
    }
}

void EpithelialCell::UpdateInfectious() {
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
    return 1 / kDivisionTime * world.prev_counts.healthy / world.prev_counts.dead;
}

double EpithelialCell::GetLocalDivisionRate(World& world) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // skip current cell
            if (i == 0 && j == 0) continue;

            // if the neighbour is healthy and is time to divide,
            int new_x = return_in_bounds_x(x + i);
            int new_y = return_in_bounds_y(y + j);
            EpithelialState neighbour_state = world.epithelial_cells[new_x][new_y]->state;
            if (neighbour_state == EpithelialState::HEALTHY && time_left_to_divide == 0) {
                // return uniform probability for each neighbour
                return 1.0 / 8.0;
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