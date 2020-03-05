#ifndef EPITHELIAL_CELL_H
#define EPITHELIAL_CELL_H

class World;

enum class EpithelialState {
    HEALTHY,
    INFECTED,
    EXPRESSING,
    INFECTIOUS,
    DEAD
};

class EpithelialCell {
public:
    int x, y;
    int age;
    int infect_time;
    int time_left_to_divide;
    EpithelialState prev_state, state;

    EpithelialCell(int x, int y, int age, int infect_time, int time_left_to_divide, EpithelialState state);
    
    void Update(World& world);
    void FlipStates();

private:
    void UpdateHealthy(World& world);
    void UpdateInfected();
    void UpdateExpressing();
    void UpdateInfectious();
    void UpdateDead(World& world);

    // division rates
    double GetGlobalDivisionRate(World& world);
    double GetLocalDivisionRate(World& world);

    int IsDeadFromOldAge();
    int IsDeadFromInfection();
};

#endif