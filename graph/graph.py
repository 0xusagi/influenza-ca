import matplotlib.pyplot as plt
import numpy as np
import sys


def plot():
    pass


# Run as main and read data from file
if __name__ == "__main__":
    n_files = len(sys.argv) - 1

    line_names = []
    data = {}
    line_count = 0

    is_first = True
    # First line is the list of all headers
    for i in range(n_files):
        filename = sys.argv[i + 1]
        with open(filename, "r") as f:
            for line in f:
                # First file to be read, so need to initialise
                if is_first:
                    if line_count == 0:
                        line_names = line.split(',')
                        for name in line_names:
                            data[name] = []

                    else:
                        split_line = line.split(',')
                        for name, val in zip(line_names, split_line):
                            data[name].append(float(val))

                else:
                    if line_count == 0:
                        line_names = line.split(',')

                    else:
                        split_line = line.split(',')
                        col = 0
                        for name, val in zip(line_names, split_line):
                            data[name][col] += float(val)
                            col += 1

                line_count += 1

    # average the values
    for name in data:
        for i in range(len(data[name])):
            data[name][i] /= n_files

    t = line_count - 1
    x = np.arange(t)
    x_ticks = np.arange(0, t, 24 * 6)
    x_labels = np.arange(0, t // (24 * 6) + 1)
    plt.xticks(x_ticks, x_labels)

    for key in data.keys():
        plt.plot(x, data[key], label=key)

    plt.legend(loc='upper right')
    plt.xlabel('time (days)')
    plt.ylabel('% total cells')

    plt.savefig('influenza.png')
