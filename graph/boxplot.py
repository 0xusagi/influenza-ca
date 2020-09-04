import matplotlib.pyplot as plt
import numpy as np
import sys

ext = "-out.out"
concentrations = ["0.001", "0.01", "0.1"]
times = ["0", "10", "24", "48"]
colors = ["blue", "red", "green"]


def set_box_color(bp, color):
    plt.setp(bp['boxes'], color=color)
    plt.setp(bp['whiskers'], color=color)
    plt.setp(bp['caps'], color=color)
    plt.setp(bp['medians'], color=color)

if __name__ == "__main__":
    n = int(sys.argv[1])
    base = sys.argv[2]

    data = {}
    for concentration in concentrations:
        data[concentration] = {}
        for time in times:
            data[concentration][time] = []
            for i in range(n):
                filename = base + "{}ext-{}h-norandom{}-out.out".format(concentration, time, i+1)

                d = {}
                with open(filename, "r") as f:
                    line_count = 0
                    line_names = []
                    for line in f:
                        line = line.rstrip()

                        if line_count == 0:
                            line_names = line.split(',')

                            for name in line_names:
                                d[name] = []

                        else:
                            line = line.split(',')
                            
                            for name, val in zip(line_names, line):
                                d[name].append(float(val))

                        line_count += 1

                data[concentration][time].append(d)

    boxplots = []
    j = 0
    offsets = [0.4,0,-0.4]
    for color, concentration in zip(colors, concentrations):
        boxplot_data = []
        for time in times:
            d = []
            for i in range(n):
                d.append(max(data[concentration][time][i]['dead']))

            boxplot_data.append(d)

        positions = np.array(range(len(boxplot_data)))*3
        positions += j
        positions = positions + offsets[j]
        boxplot = plt.boxplot(boxplot_data, positions=positions, sym='', widths=0.6)
        set_box_color(boxplot, color)

        plt.plot([], c=color, label=concentration+"%")

        j += 1

    plt.legend()

    plt.xticks(np.array(range(0, len(times) * 3, 3)) + 1, times)
    plt.xlabel('% dead epithelial cells')
    plt.ylabel('time of introduction of DIPs')
    plt.show()
