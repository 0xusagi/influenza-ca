#include "config.h"
#include "input.h"
#include "window.h"
#include "world.h"
#include "utils.h"

void init() {
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialise SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // parse config
    printf("Parsing config...\n");
    parse_config();
}

int main(int argc, char *argv[]) {
    // parse cmd line options
    struct cmd_opts options = parse_cmd_opts(argc, argv);

    // initialise everything
    init();

    // create the window for graphics
    Window window(options.graphics);

    // create input
    Input input;

    FILE* fp;
    fp = fopen(options.output_filename, "w");

    // print the headers
    fprintf(fp, "healthy,infected,dead,immune\n");

    printf("Initialising...\n");
    World world = World(fp);
    window.Draw(world);

    printf("Starting simulation...\n");
    // start simulation
    for (int t = 0; t < kSimulationLength && !input.quit; t++) {
        // poll for input
        input.Poll();

        world.Step(fp);
        window.Draw(world);
    }

    fclose(fp);

    return 0;
}
