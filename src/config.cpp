#include "config.h"

#include <sstream>
#include <string>
#include <fstream>
#include <string>

int kGridWidth;
int kGridHeight;
double kFlowRate;
double kImmLifespan;
double kCellLifespan;
double kInfectLifespan;
double kInfectInit;
double kInfectRate;
double kInfectDelay;
double kExpressDelay;
double kDivisionTime;
double kBaseImmCell;
double kRecruitDelay;
double kRecruitment;
double kTotalEpithelialCells;
int kSimulationLength;
int kNumSimulations;

void store_line(std::string key, std::string value) {
    if (key == "grid_width")
        kGridWidth = std::stoi(value);

    else if (key == "grid_height")
        kGridHeight = std::stoi(value);

    else if (key == "flow_rate")
        kFlowRate = std::stod(value);
        
    else if (key == "imm_lifespan")
        kImmLifespan = std::stod(value);

    else if (key == "cell_lifespan")
        kCellLifespan = std::stod(value);

    else if (key == "infect_lifespan")
        kInfectLifespan = std::stod(value);

    else if (key == "infect_init")
        kInfectInit = std::stod(value);

    else if (key == "infect_rate")
        kInfectRate = std::stod(value);

    else if (key == "infect_delay")
        kInfectDelay = std::stod(value);

    else if (key == "express_delay")
        kExpressDelay = std::stod(value);

    else if (key == "division_time")
        kDivisionTime = std::stod(value);

    else if (key == "base_imm_cell")
        kBaseImmCell = std::stod(value);
        
    else if (key == "recruit_delay")
        kRecruitDelay = std::stod(value);

    else if (key == "recruitment")
        kRecruitment = std::stod(value);

    else if (key == "simulation_hours")
        kSimulationLength = std::stod(value);
    
    else if (key == "num_simulations") 
        kNumSimulations = std::stoi(value);
}

void parse_config() {
    std::ifstream infile("simulation.conf");

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                store_line(key, value);
            }
        }
    }

    // print config
    print_config();
    
    // normalise
    kImmLifespan *= kFlowRate;
    kCellLifespan *= kFlowRate;
    kInfectLifespan *= kFlowRate;
    kInfectRate /= kFlowRate;
    kInfectDelay *= kFlowRate;
    kExpressDelay *= kFlowRate;
    kDivisionTime *= kFlowRate;

    kTotalEpithelialCells = kGridWidth * kGridHeight;
    kBaseImmCell *= kTotalEpithelialCells;
    kSimulationLength *= kFlowRate;
}

void print_config() {
    printf("Starting simulations with these configurations...\n");
    printf("- Grid width: %d\n", kGridWidth);
    printf("- Grid height: %d\n", kGridHeight);
    printf("- Flow rate: %f\n", kFlowRate);
    printf("- Immune lifespan: %f\n", kImmLifespan);
    printf("- Cell lifespan: %f\n", kCellLifespan);
    printf("- Infect lifespan: %f\n", kInfectLifespan);
    printf("- Infect init: %f\n", kInfectInit);
    printf("- Infect rate: %f\n", kInfectRate);
    printf("- Infect delay: %f\n", kInfectDelay);
    printf("- Express delay: %f\n", kExpressDelay);
    printf("- Division time: %f\n", kDivisionTime);
    printf("- Base immune cells: %f\n", kBaseImmCell);
    printf("- Recruit delay: %f\n", kRecruitDelay);
    printf("- Recruitment: %f\n", kRecruitment);
    printf("- Simulation length: %dh\n", kSimulationLength);
    printf("- Number of simulations: %d\n", kNumSimulations);
}