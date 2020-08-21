import matplotlib.pyplot as plt
import numpy as np
import sys


def plot():
    pass


# Run as main and read data from file
if __name__ == "__main__":
    first_file = sys.argv[1]
    second_file = sys.argv[2]

    first_data = {}
    second_data = {}

    line_names = []
    line_count = 0
    with open(first_file, "r") as f:
        for line in f:
            if line_count == 0:
                line_names = line.rstrip().split(',')
                for name in line_names:
                    first_data[name] = []

            else:
                split_line = line.split(',')
                for name, val in zip(line_names, split_line):
                    first_data[name].append(float(val))

            line_count += 1

    line_count = 0
    with open(second_file, "r") as f:
        for line in f:
            if line_count == 0:
                line_names = line.rstrip().split(',')
                for name in line_names:
                    second_data[name] = []

            else:
                split_line = line.split(',')
                for name, val in zip(line_names, split_line):
                    second_data[name].append(float(val))

            line_count += 1

    t = line_count - 1
    x = np.arange(t)
    x_ticks = np.arange(0, t, 24 * 6)
    x_labels = np.arange(0, t // (24 * 6) + 1)
    plt.xticks(x_ticks, x_labels, fontsize=12)

    y_ticks = [y / 10 for y in range(11)]
    y_labels = [y / 10 for y in range(11)]
    plt.yticks(y_ticks, y_labels, fontsize=12)


    fig, axs = plt.subplots(3,2)
    plt.setp(axs, xticks=x_ticks, xticklabels=x_labels)

    axs = axs.flat

    i = 0
    for key in line_names:
        axs[i].set_title(key)
        axs[i].plot(x, first_data[key], label="random")
        axs[i].plot(x, second_data[key], label="at site", linestyle="dashed")

        i += 1

    for ax in axs:
        ax.set_xlabel('time (days)')
        ax.set_ylabel('% total cells')

    handles, labels = axs[0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='lower center')
    fig.tight_layout()
    fig.savefig('compare.png', dpi=600)
