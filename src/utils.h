#ifndef UTILS_H
#define UTILS_H

#include <random>

/* Random */
extern std::mt19937_64 rng;

extern std::uniform_int_distribution<int> dx_distribution;
extern std::uniform_int_distribution<int> dy_distribution;

extern std::uniform_real_distribution<double> p_distribution;

int random_dx();
int random_dy();
double random_p();

/* Return in bounds x and y coordinates */
int return_in_bounds_x(int x);
int return_in_bounds_y(int y);

int is_out_of_bounds_x(int x);
int is_out_of_bounds_y(int y);

/* Command line options */
struct cmd_opts {
    int graphics;
    char* base_name;
    char* output_filename;
    char* section_filename;
    char* patches_filename;
};

void parse_cmd_opts(int argc, char** argv, struct cmd_opts *opts);

#endif
