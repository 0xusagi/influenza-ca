#include "config.h"
#include "window.h"
#include "world.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    // parse cmd line options
    struct cmd_opts options = parse_cmd_opts(argc, argv);

    // parse config
    printf("Parsing config...\n");
    parse_config();

    // create the window for graphics
    Window window(options.graphics);

    FILE* fp;
    fp = fopen(options.output_filename, "w");

    // print the headers
    fprintf(fp, "healthy,infected,dead,immune\n");

    printf("Initialising...\n");
    World world = World(fp);
    window.Draw(world);

    printf("Starting simulation...\n");
    // start simulation
    for (int t = 0; t < kSimulationLength; t++) {
        world.Step(fp);
        window.Draw(world);
    }

    fclose(fp);

    return 0;
}
