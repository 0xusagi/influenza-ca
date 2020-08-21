#include "config.h"
#include "utils.h"

#include <ctype.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

std::mt19937_64 rng(time(NULL));
std::uniform_int_distribution<int> dx_distribution(-1, 1);
std::uniform_int_distribution<int> dy_distribution(-1, 1);
std::uniform_real_distribution<double> p_distribution(0.0, 1.0);

int random_dx() {
    return dx_distribution(rng);
}

int random_dy() {
    return dy_distribution(rng);
}

double random_p() {
    return p_distribution(rng);
}

int return_in_bounds_x(int x) {
    if (x >= kGridWidth) {
        return 0;
    }
    else if (x < 0) {
        return kGridWidth - 1;
    }

    return x;
}

int return_in_bounds_y(int y) {
    // No toroidal condition for y boundary so just return the out of bounds
    if (!kToroidalY) {
        return y;
    }

    if (y >= kGridHeight) {
        return 0;
    }
    else if (y < 0) {
        return kGridHeight - 1;
    }

    return y;
}

int is_out_of_bounds_x(int x) {
    return x >= kGridWidth || x < 0;
}

int is_out_of_bounds_y(int y) {
    return y >= kGridHeight || y < 0;
}

void parse_cmd_opts(int argc, char** argv, struct cmd_opts *opts) {
    opts->graphics = 0;
    opts->output_filename = "";
    opts->section_filename = "";

    printf("Starting application with flags: \n");

    int c;
    while ((c = getopt(argc, argv, "gn:")) != -1) {
        switch (c) {
            case 'g':
                printf("\tgraphics\n");
                opts->graphics = 1;
                break;

            case 'n':
                int optarg_len = strlen(optarg);
                opts->base_name = new char[optarg_len + 1];
                strcpy(opts->base_name, optarg);

                const char* output_filename_suffix = "-out.out";
                opts->output_filename = new char[optarg_len + strlen(output_filename_suffix) + 1];
                strcpy(opts->output_filename, optarg);
                strcat(opts->output_filename, output_filename_suffix);

                const char* section_filename_suffix = "-section.out";
                opts->section_filename = new char[optarg_len + strlen(section_filename_suffix) + 1];
                strcpy(opts->section_filename, optarg);
                strcat(opts->section_filename, section_filename_suffix);

                printf("\tbase name: %s\n", opts->base_name);
                printf("\toutput file: %s\n", opts->output_filename);
                printf("\tsection output file: %s\n", opts->section_filename);
                break;
        }
    }
}
