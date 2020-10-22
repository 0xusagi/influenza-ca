import matplotlib.pyplot as plt
import numpy as np
import sys

ext = "-out.out"
locations = [2500, 5000, 7500]
max_height = 10000

if __name__ == "__main__":
    n = int(sys.argv[1])
    base = sys.argv[2]
    col_name = sys.argv[3]

    data = {}
    for loc in locations:
        data[loc] = []
        for i in range(n):
            d = {}
            if loc == 5000:
                filename = base + "dip{}-{}h{}-out.out".format(0.1, 0, i+1)
            else:
                filename = base + "dip{}-{}h-{}{}-out.out".format(0.1, 0, loc, i+1)

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

                data[loc].append(d)


    boxplot_data = []
    for loc in locations:
        bd = []
        for i in range(n):
            bd.append(max(data[loc][i][col_name]))

        boxplot_data.append(bd)

    print(boxplot_data)

    plt.boxplot(boxplot_data)

    plt.ylim(bottom=0)
    plt.xticks([1, 2, 3], [loc / max_height for loc in locations])
    plt.xlabel('Range of initialisation of DI influenza viruses (% total height)')
    plt.ylabel('% dead epithelial cells')

    plt.savefig('boxplot-location.png', dpi=700)
