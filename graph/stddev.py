import matplotlib.pyplot as plt
import numpy as np
import sys
from statistics import stdev, mean


def plot():
    pass


# Run as main and read data from file
if __name__ == "__main__":
    n_files = int(sys.argv[1])
    base_filename = sys.argv[2]

    line_names = []
    data = {}

    # First line is the list of all headers
    for i in range(n_files):
        filename = base_filename + str(i+1) + "-out.out"
        line_count = 0
        with open(filename, "r") as f:
            for line in f:
                # First file to be read, so need to initialise
                if i == 0:
                    if line_count == 0:
                        line_names = line.rstrip().split(',')
                        for name in line_names:
                            data[name] = []

                        # Add a new column called total infected
                        data['total-infected'] = []

                    else:
                        split_line = line.split(',')
                        total_infected = 0
                        for name, val in zip(line_names, split_line):
                            data[name].append([float(val)])

                            if name == "stv-infected" or name == "dip-infected" or name == "co-infected":
                                total_infected += float(val)

                        data['total-infected'].append([total_infected])

                else:
                    if line_count == 0:
                        line_names = line.rstrip().split(',')

                    else:
                        split_line = line.split(',')
                        total_infected = 0
                        for name, val in zip(line_names, split_line):
                            data[name][line_count - 1].append(float(val))

                            if name == "stv-infected" or name == "dip-infected" or name == "co-infected":
                                total_infected += float(val)

                        data['total-infected'][line_count - 1].append(total_infected)

                line_count += 1

    # average the values
    averages = {}
    stdevs = {}
    for name in data:
        averages[name] = []
        stdevs[name] = []
        for i in range(len(data[name])):
            averages[name].append(mean(data[name][i]))
            stdevs[name].append(stdev(data[name][i]))

    print(averages, stdevs)


    t = line_count - 1
    x = np.arange(t)
    x_ticks = np.arange(0, t, 24 * 6)
    print(x_ticks)
    x_labels = np.arange(0, t // (24 * 6) + 1)
    print(x_labels)
    plt.xticks(x_ticks, x_labels, fontsize=12)

    y_ticks = [y / 10 for y in range(11)]
    y_labels = [y / 10 for y in range(11)]
    plt.yticks(y_ticks, y_labels, fontsize=12)

    for key in averages.keys():
        if key == "dip-infected" or key == "co-infected":
            continue
        
        # change linestyle for total infected cells
        if key != "total-infected":
            lower = np.array(averages[key]) - np.array(stdevs[key])
            upper = np.array(averages[key]) + np.array(stdevs[key])

            lower[lower < 0] = 0
            upper[upper > 1] = 1

            plt.plot(x, averages[key], label=key if key != "stv-infected" else "infected")
            plt.fill_between(x, lower, upper)

    plt.legend(loc='upper right')
    plt.xlabel('time (days)', fontsize=12)
    plt.ylabel('% total cells', fontsize=12)

    plt.savefig('stdevs.png')
