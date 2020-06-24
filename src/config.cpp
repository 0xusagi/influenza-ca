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
double kStvInfectInit;
double kDipInfectInit;
double kDeadInit;
double kStvErrorRate;
double kCoErrorRate;
double kStvInfectRate;
double kCoInfectRate;
double kStvInfectDelay;
double kCoInfectDelay;
double kStvExpressDelay;
double kCoExpressDelay;
double kDivisionTime;
double kBaseImmCell;
double kRecruitDelay;
double kRecruitment;
double kTotalEpithelialCells;

double kDipExtInit;
double kDipExtTime;

int kGlobalEpithelialDivision;
int kSimulationLength;

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

    else if (key == "stv_infect_init")
        kStvInfectInit = std::stod(value);

    else if (key == "dip_infect_init")
        kDipInfectInit = std::stod(value);

    else if (key == "dead_init")
        kDeadInit = std::stod(value);

    else if (key == "stv_error_rate")
        kStvErrorRate = std::stod(value);

    else if (key == "co_error_rate")
        kCoErrorRate = std::stod(value);

    else if (key == "stv_infect_rate")
        kStvInfectRate = std::stod(value);

    else if (key == "co_infect_rate")
        kCoInfectRate = std::stod(value);

    else if (key == "stv_infect_delay")
        kStvInfectDelay = std::stod(value);

    else if (key == "co_infect_delay")
        kCoInfectDelay = std::stod(value);

    else if (key == "stv_express_delay")
        kStvExpressDelay = std::stod(value);

    else if (key == "co_express_delay")
        kCoExpressDelay = std::stod(value);

    else if (key == "division_time")
        kDivisionTime = std::stod(value);

    else if (key == "base_imm_cell")
        kBaseImmCell = std::stod(value);
        
    else if (key == "recruit_delay")
        kRecruitDelay = std::stod(value);

    else if (key == "recruitment")
        kRecruitment = std::stod(value);

    else if (key == "dip_ext_init")
        kDipExtInit = std::stod(value);

    else if (key == "dip_ext_time")
        kDipExtTime = std::stod(value);

    else if (key == "global_epithelial_division") 
        kGlobalEpithelialDivision = std::stoi(value);

    else if (key == "simulation_hours")
        kSimulationLength = std::stod(value);
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
    kStvInfectRate /= kFlowRate;
    kCoInfectRate /= kFlowRate;
    kStvInfectDelay *= kFlowRate;
    kCoInfectDelay *= kFlowRate;
    kStvExpressDelay *= kFlowRate;
    kCoExpressDelay *= kFlowRate;
    kDivisionTime *= kFlowRate;

    kDipExtTime *= kFlowRate;

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
    printf("- STV infect init: %f\n", kStvInfectInit);
    printf("- DIP infect init: %f\n", kDipInfectInit);
    printf("- Dead init: %f\n", kDeadInit);
    printf("- STV error rate: %f\n", kStvErrorRate);
    printf("- Co error rate: %f\n", kCoErrorRate);
    printf("- STV infect rate: %f\n", kStvInfectRate);
    printf("- Co infect rate: %f\n", kCoInfectRate);
    printf("- STV infect delay: %f\n", kStvInfectDelay);
    printf("- Co infect delay: %f\n", kCoInfectDelay);
    printf("- STV express delay: %f\n", kStvExpressDelay);
    printf("- Co express delay: %f\n", kCoExpressDelay);
    printf("- Division time: %f\n", kDivisionTime);
    printf("- Base immune cells: %f\n", kBaseImmCell);
    printf("- Recruit delay: %f\n", kRecruitDelay);
    printf("- Recruitment: %f\n", kRecruitment);
    printf("- DIP external init: %f\n", kDipExtInit);
    printf("- DIP external time: %f\n", kDipExtTime);
    printf("- Global epithelial cell division: %d\n", kGlobalEpithelialDivision);
    printf("- Simulation length: %dh\n", kSimulationLength);
}
