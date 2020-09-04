import matplotlib.pyplot as plt
import numpy as np
import sys

suffix = "-patches.out"

if __name__ == "__main__":
    basefilename = sys.argv[1]

    n = int(sys.argv[2])

    proportion = 0

    healthy_total_max_patch = 0
    healthy_patches_count = []

    dead_total_max_patch = 0
    dead_patches_count = []

    for i in range(n):
        filename = basefilename + str(i+1) + suffix

        # TODO Currently only handles one line
        # read the patches output file
        with open(filename, "r") as f:
            for line in f:
                line = line.rstrip().split('+')

                proportion += float(line[0])
                healthy_part = line[1].split(',')
                dead_part = line[2].split(',')

                # store healthy part
                healthy_patches_count.append(len(line[1]))

                healthy_max_size = -1
                for num in healthy_part:
                    num = int(num)
                    if num > healthy_max_size:
                        healthy_max_size = num

                # store dead part
                dead_patches_count.append(len(line[2]))
                dead_max_size = -1
                for num in dead_part:
                    num = int(num)
                    if num > dead_max_size:
                        dead_max_size = num

                # store dead part

            healthy_total_max_patch += healthy_max_size
            dead_total_max_patch += dead_max_size

    # average proportion
    proportion /= n

    # average the healthy max patch size
    healthy_max_patch_size = healthy_total_max_patch / n
    dead_max_patch_size = dead_total_max_patch / n

    # average the dead max patch size
    healthy_avg_patches = sum(healthy_patches_count) / n
    dead_avg_patches = sum(dead_patches_count) / n

    print("Proportion of healthy neighbour per dead cell: " + str(proportion))

    print("Healthy biggest patch size: " + str(healthy_max_patch_size))
    print("Average number of healthy patches per simulation: " + str(healthy_avg_patches))

    print("Dead biggest patch size: " + str(dead_max_patch_size))
    print("Average numbmer of dead patches per simulation: " + str(dead_avg_patches))
