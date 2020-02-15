#include "config.h"
#include "world.h"
#include "utils.h"

void run_simulations(struct cmd_opts options) {
    FILE* fp;
    fp = fopen(options.output_filename, "w");

    for (int t = 1; t <= kNumSimulations; t++) {
        printf("Simulation for round %d\n", t);

        // print the round number
        fprintf(fp, "Round: %d\n", t);

        printf("Initialising...\n");
        World world = World();

        printf("Starting simulation...\n");
        world.Simulate(fp);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    // parse cmd line options
    struct cmd_opts options = parse_cmd_opts(argc, argv);

    // parse config
    printf("Parsing config...\n");
    parse_config();

    run_simulations(options);

    return 0;
}