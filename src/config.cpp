#include "config.h"

#include <sstream>
#include <string>
#include <fstream>
#include <string>
#include <string.h>

int kGridWidth;
int kGridHeight;
double kFlowRate;
double kImmLifespan;
double kCellLifespan;
double kInfectLifespan;
double kStvInfectInit;
double kStvInfectInitLoc;
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
double kDipExtLoc;

int kNumSections;

double neighbour_infect_multiplier[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };

int kToroidalY;

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

    else if (key == "stv_infect_init_loc")
        kStvInfectInitLoc = std::stod(value);

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

    else if (key == "dip_ext_loc") 
        kDipExtLoc = std::stod(value);

    else if (key == "n_sections")
        kNumSections = std::stoi(value);

    else if (key == "neighbour_infect_multiplier") {
        char* s = new char[value.size() + 1];
        std::copy(value.begin(), value.end(), s);
        s[value.size()] = '\0';

        // do strtok operation to get the values
        char* token;
        char* delim = " ";
        token = strtok(s, delim);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                neighbour_infect_multiplier[i][j] = atof(token);
                token = strtok(NULL, " ");
            }
        }

        delete s;
    }

    else if (key == "toroidal_y") 
        kToroidalY = std::stoi(value);

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
    printf("- STV infect location: %f\n", kStvInfectInitLoc);
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
    printf("- DIP external init location: %f\n", kDipExtLoc);
    printf("- Number of sections: %d\n", kNumSections);

    printf("- Neighbour infect multiplier:\n");
    for (int i = 0; i < 3; i++) {
        printf("\t%f %f %f\n", neighbour_infect_multiplier[i][0], neighbour_infect_multiplier[i][1], neighbour_infect_multiplier[i][2]);
    }

    printf("- Global epithelial cell division: %d\n", kGlobalEpithelialDivision);
    printf("- Simulation length: %dh\n", kSimulationLength);
}
