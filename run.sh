#!/bin/bash

usage() {
    echo "Usage: $0 [-ng]" 1>&2;
    exit 1;
}

# get arguments additional arguments
args=""
while getopts "gn:" arg; do
    case $arg in
        n)
            n=${OPTARG}
            ;;
        g) 
            args="${args} -g"
            ;;
        *)
            usage
            ;;
    esac
done

# check if value exists
if [ -z "${n}" ]; then
    usage
fi

all_out_names=""

echo $args

declare -i i
i=1
while [ $i -le $n ]
do
    outname="out${i}.csv"

    # run the simulation
    ./build/simulation -f $outname $args

    if [ $? -ne "0" ]; then
        exit 1
    fi

    all_outnames="${all_outnames} ${outname}"
    i+=1
done

echo $all_out_names

# plot the graph

