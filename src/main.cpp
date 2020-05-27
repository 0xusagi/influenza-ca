#include "config.h"
#include "window.h"
#include "world.h"
#include "utils.h"

void run_simulations(struct cmd_opts options, Window& window) {
    FILE* fp;
    fp = fopen(options.output_filename, "w");

    for (int t = 1; t <= kNumSimulations; t++) {
        printf("Simulation for round %d\n", t);

        // print the headers
        fprintf(fp, "healthy,infected,dead,immune\n");

        printf("Initialising...\n");
        World world = World();

        printf("Starting simulation...\n");
        world.Simulate(fp, window);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    // parse cmd line options
    struct cmd_opts options = parse_cmd_opts(argc, argv);

    // parse config
    printf("Parsing config...\n");
    parse_config();

    // create the window for graphics
    Window window(options.graphics);

    run_simulations(options, window);

quit:
    return 0;
}
