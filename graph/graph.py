import matplotlib.pyplot as plt
import numpy as np
import sys


def plot():
    pass


# Run as main and read data from file
if __name__ == "__main__":
    line_names = []
    data = {}
    line_count = 0
    # First line is the list of all headers
    with open(sys.argv[1], "r") as f:
        for line in f:
            if line_count == 0:
                line_names = line.split(',')
                for name in line_names:
                    data[name] = []

            else:
                split_line = line.split(',')
                for name, val in zip(line_names, split_line):
                    data[name].append(float(val))

            line_count += 1

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
