#!/bin/bash

# number of simulations to run
n_simulations=$1

all_out_names=""

i=1
while [ $i -le $n_simulations ]
do
    out_name="out${i}.csv"

    # run the simulation

    all_out_names="${all_out_names} ${out_name}"
    true $(( i++ ))
done

echo $all_out_names

# plot the graph
