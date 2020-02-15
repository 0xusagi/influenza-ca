#include "config.h"
#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

std::mt19937_64 rng(time(NULL));
std::uniform_int_distribution dx_distribution(-1, 1);
std::uniform_int_distribution dy_distribution(-1, 1);
std::uniform_real_distribution p_distribution(0.0, 1.0);

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
    if (y >= kGridHeight) {
        return 0;
    }
    else if (y < 0) {
        return kGridHeight - 1;
    }

    return y;
}

struct cmd_opts parse_cmd_opts(int argc, char** argv) {
    struct cmd_opts opts;
    opts.graphics = 0;
    opts.output_filename = "";

    printf("Starting application with flags: \n");

    int c;
    while ((c = getopt(argc, argv, "gf:")) != -1) {
        switch (c) {
            case 'g':
                printf("\tgraphics\n");
                opts.graphics = 1;
                break;
            case 'f':
                printf("\toutput file: %s\n", optarg);
                opts.output_filename = optarg;
                break;
        }
    }

    return opts;
}