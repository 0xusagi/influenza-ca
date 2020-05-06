#ifndef EPITHELIAL_CELL_H
#define EPITHELIAL_CELL_H

class World;

enum class EpithelialState {
    HEALTHY,
    S_INFECTED,
    S_EXPRESSING,
    S_INFECTIOUS,
    D_INFECTED,
    C_INFECTED,
    C_EXPRESSING,
    C_INFECTIOUS,
    DEAD
};

class EpithelialCell {
public:
    int x, y;
    int age;
    int infect_time;
    int time_left_to_divide;
    EpithelialState prev_state, state;

    EpithelialCell(int x, int y, int age, int infect_time, int time_left_to_divide);
    
    void Update(World& world);
    void FlipStates();

private:
    void UpdateHealthy(World& world);
    void UpdateSTVInfected();
    void UpdateSTVExpressing();
    void UpdateSTVInfectious();
    void UpdateDIPInfected();
    void UpdateCoInfected();
    void UpdateCoExpressing();
    void UpdateCoInfectious();
    void UpdateDead(World& world);

    // division rates
    double GetGlobalDivisionRate(World& world);
    double GetLocalDivisionRate(World& world);

    int IsDeadFromOldAge();
    int IsDeadFromInfection();
};

#endif
