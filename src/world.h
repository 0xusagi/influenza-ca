#ifndef WORLD_H 
#define WORLD_H 

#include <stdio.h>
#include <random>

class EpithelialCell;

typedef struct {
    int healthy;
    int infected;
    int dead;
    int immune;
} counts_t;

class World {
public:
    counts_t prev_counts;
    counts_t counts;
    EpithelialCell*** epithelial_cells; 

    World();
    ~World();

    void Simulate(FILE* fp);

private:
    void PrintTimeStepToFile(FILE* fp);

    std::uniform_int_distribution<int> x_distribution;
    int RandomX();
    std::uniform_int_distribution<int> y_distribution;
    int RandomY();

    std::uniform_int_distribution<int> epithelial_age_distribution;
    int RandomEpithelialAge();
    std::uniform_int_distribution<int> immune_age_distribution;
    int RandomImmuneAge();
};

#endif