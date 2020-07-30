#ifndef CONFIG_H
#define CONFIG_H


extern int kGridWidth;
extern int kGridHeight;
extern double kFlowRate;
extern double kImmLifespan;
extern double kCellLifespan;
extern double kInfectLifespan;
extern double kStvInfectInit;
extern double kStvInfectInitLoc;
extern double kDipInfectInit;
extern double kDeadInit;
extern double kStvErrorRate;
extern double kCoErrorRate;
extern double kStvInfectRate;
extern double kCoInfectRate;
extern double kStvInfectDelay;
extern double kCoInfectDelay;
extern double kStvExpressDelay;
extern double kCoExpressDelay;
extern double kDivisionTime;
extern double kBaseImmCell;
extern double kRecruitDelay;
extern double kRecruitment;
extern int kGlobalEpithelialDivision;

extern double kDipExtInit;
extern double kDipExtTime;
extern double kDipExtLoc;

extern int kNumSections;

extern double kTotalEpithelialCells;
extern int kSimulationLength;

extern double neighbour_infect_multiplier[3][3];

extern int kToroidalY;

void parse_config();
void print_config();

#endif
