# Influenza Cellular Automaton
Cellular automaton model for investigating the spatial temporal effects of DI influenza viruses on the dynamics of the influenza A viral infection. The cellular automaton model is based on the models proposed by Beauchemin et al. [1] and Beauchemin [2].

## Building the Code
The code has only been tested on macOS with the following dependencies:
1. [SDL2](https://www.libsdl.org/)
1. [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
1. [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)

To build the project:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Running the code
Before running the code, the configuration file must first be created as shown in the [Configurations](#Configurations) section.

Run the code without graphics for 10 runs using:
```
./run.sh -n 10 -s simulation
```

Run the code with graphics for 10 runs using:
```
./run.sh -n 10 -s simulation -g
```

The output files are saved in the `out` directory which will be created if it does not exist.
Created output files are:
1. {basefilename}{run number}-out.out     - proportion of cells per cell type
1. {basefilename}{run number}-section.out - proportion of cells split according to sections
1. {basefilename}{run number}-patches.out - statistics for cell counts at the time specified by count_patch_hour

Run with an offset 4 for 5 runs to modify run number using:
```
./run.sh -n 5 -s simulation -o 4
```

### Configurations 
Configurations must be put into a file called `simulation.conf` in the same directory as the executable.

Default parameters:

```
grid_width=280
grid_height=10000
flow_rate=6
imm_lifespan=168
cell_lifespan=380
infect_lifespan=24
stv_infect_init=0.01
stv_infect_init_loc=5000
dip_infect_init=0
dead_init=0
stv_error_rate=0.001
co_error_rate=0
stv_infect_rate=2
co_infect_rate=2
stv_infect_delay=6
co_infect_delay=6
stv_express_delay=4
co_express_delay=4
division_time=12
base_imm_cell=0.00015
recruit_delay=42
recruitment=0.25
dip_ext_init=0
dip_ext_time=0
dip_ext_loc=7500
n_sections=10
neighbour_infect_multiplier=0.0001 0.0001 0.0001 1 1 1 2 2 2
toroidal_y=0
graphics_start_x=10
graphics_start_y=100
simulation_hours=288
global_epithelial_division=0
random_immune_cell_spawn=0
count_patch_hour=120
```

## References
1. Beauchemin, C., Samuel, J. & Tuszynski, J. A simple cellular automaton model for influenza A viral infections. Journal of Theoretical Biology 232, 223–234. issn: 00225193. https:// linkinghub.elsevier.com/retrieve/pii/S0022519304003819 (Jan. 2005).
1. Beauchemin, C. Probing the effects of the well-mixed assumption on viral infection dynam- ics. Journal of Theoretical Biology 242, 464–477. issn: 00225193. https://linkinghub. elsevier.com/retrieve/pii/S0022519306001196 (Sept. 2006).
