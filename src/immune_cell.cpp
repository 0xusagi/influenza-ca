#include "config.h"
#include "epithelial_cell.h"
#include "immune_cell.h"
#include "utils.h"
#include "world.h"

ImmuneCell::ImmuneCell(int x, int y, int age, ImmuneState state) 
    : x(x)
    , y(y)
    , age(age)
    , state(state) {}

void ImmuneCell::Update(World& world) {
    age++;

    // dont update for inactive cell
    if (age < 0) return;

    // move
    Move();

    // update
    if (state == ImmuneState::VIRGIN) {
        UpdateVirgin(world);
    }
    else if (state == ImmuneState::MATURE) {
        UpdateMature(world);
    }
}

void ImmuneCell::UpdateVirgin(World& world) {
    // check dead
    if (IsDeadFromOldAge()) {
        state = ImmuneState::DEAD;
        return;
    }

    // recognition event
    EpithelialState epithelial_state = world.epithelial_cells[x][y]->prev_state;
    if (epithelial_state == EpithelialState::S_EXPRESSING || epithelial_state == EpithelialState::S_INFECTIOUS ||
        epithelial_state == EpithelialState::D_INFECTED || 
        epithelial_state == EpithelialState::C_EXPRESSING || epithelial_state == EpithelialState::C_INFECTIOUS) {
        state = ImmuneState::MATURE;
        return;
    }
}

void ImmuneCell::UpdateMature(World& world) {
    // check dead
    if (IsDeadFromOldAge()) {
        state = ImmuneState::DEAD;
        return;
    }

    // recognition event
    EpithelialState epithelial_state = world.epithelial_cells[x][y]->prev_state;
    if (epithelial_state == EpithelialState::S_EXPRESSING || epithelial_state == EpithelialState::S_INFECTIOUS ||
        epithelial_state == EpithelialState::D_INFECTED || 
        epithelial_state == EpithelialState::C_EXPRESSING || epithelial_state == EpithelialState::C_INFECTIOUS)  {
        world.MatureImmuneCellRecognitionEvent(x, y);
    }
}

void ImmuneCell::Move() {
    x = return_in_bounds_x(x + random_dx());
    y = return_in_bounds_y(y + random_dy());
}

int ImmuneCell::IsDeadFromOldAge() {
    if (age > kImmLifespan) {
        return 1;
    }

    return 0;
}
