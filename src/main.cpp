#include "config.h"
#include "input.h"
#include "window.h"
#include "world.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    // parse cmd line options
    struct cmd_opts options = parse_cmd_opts(argc, argv);

    // parse config
    printf("Parsing config...\n");
    parse_config();

    FILE* fp;
    fp = fopen(options.output_filename, "w");

    // create the window for graphics
    Window window(options.graphics);

    // create input
    Input input;

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

        // paused
        if (input.paused) {
            // if n is pressed, we move to the next step 
            // otherwise, we are still paused
            if (!input.IsDown(Keyboard::n)) {
                t--;

                // check if screen moved
                if (window.Move(input)) {
                    window.Draw(world);
                }

                continue;
            }
        }
        // update step
        world.Step(fp);
        window.Draw(world);
    }

    fclose(fp);

    // return 1 if quit with in the middle else 0
    if (input.quit) {
        return 1;
    }
    else {
        return 0;
    }
}
