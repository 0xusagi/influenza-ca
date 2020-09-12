#include "epithelial_cell.h"
#include "config.h"
#include "input.h"
#include "window.h"
#include "world.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    // parse cmd line options
    struct cmd_opts options;
    parse_cmd_opts(argc, argv, &options);

    // parse config
    printf("Parsing config...\n");
    parse_config();

    FILE* out_fp;
    out_fp = fopen(options.output_filename, "w");
    FILE* section_fp;
    section_fp = fopen(options.section_filename, "w");
    FILE* patches_fp;
    patches_fp = fopen(options.patches_filename, "w");

    // create the window for graphics
    Window window(options);

    // print the headers
    fprintf(out_fp, "healthy,stv-infected,dip-infected,co-infected,dead,immune\n");

    // create input
    Input input;

    printf("Initialising...\n");
    World world = World(out_fp, section_fp);
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
        world.Step(out_fp, section_fp);
        window.Draw(world);

        // time to count the dead epithelial cells
        if (t == kCountDeadPatchesHour) {
            world.PrintInfo(patches_fp);
        }
    }

    fclose(out_fp);
    fclose(section_fp);
    fclose(patches_fp);

    // return 1 if quit with in the middle else 0
    if (input.quit) {
        return 1;
    }
    else {
        return 0;
    }
}
