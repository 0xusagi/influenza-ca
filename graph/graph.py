import matplotlib.pyplot as plt
import numpy as np
import sys
from math import sqrt

cell_names = ['healthy', 'infected', 'dead', 'immune']
total_cells = int(sys.argv[2]) * int(sys.argv[3])

# Read in the data from the file
all_simulations = []
with open(sys.argv[1], "r") as f:
    for line in f:
        line = line.rstrip('\n').split(',')

        # Here is a beginning of a new simulation
        if len(line) == 1:
            sim_dict = {}
            for cell in cell_names:
                sim_dict[cell] = []
            all_simulations.append(sim_dict)
        else:
            all_simulations[-1]['healthy'].append(float(line[0]))
            all_simulations[-1]['infected'].append(float(line[1]))
            all_simulations[-1]['dead'].append(float(line[2]))
            all_simulations[-1]['immune'].append(float(line[3])) 
            
t = len(all_simulations[0]['healthy'])
n_simulations = len(all_simulations)

x = np.arange(t)
x_ticks = np.arange(0, t, 24 * 6)
y_ticks = np.arange(0.0, 1.1, 0.1)
plt.xticks(x_ticks)
plt.yticks(y_ticks)

plt.plot(x, all_simulations[0]['healthy'], '-b', label='healthy')
plt.plot(x, all_simulations[0]['infected'], '-r', label='infected')
plt.plot(x, all_simulations[0]['dead'], '-g', label='dead')
plt.plot(x, all_simulations[0]['immune'], '-y', label='immune')
plt.legend(loc='upper right')
plt.savefig('influenza.png')