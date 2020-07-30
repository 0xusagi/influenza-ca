#!/bin/bash

usage() {
    echo "Usage: $0 [-ng]" 1>&2;
    exit 1;
}

# create out directory if not exists
out_dir="out"
if [ ! -e $out_dir ]; then
    mkdir $out_dir
fi

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

all_outnames=""
all_section_outnames=""

declare -i i
i=1
while [ $i -le $n ]
do
    outname="${out_dir}/out${i}.out"
    sectionoutname="${out_dir}/section${i}.out"

    # run the simulation
    ./build/simulation -f $outname -s $sectionoutname $args

    if [ $? -ne "0" ]; then
        exit 1
    fi

    all_outnames="${all_outnames} ${outname}"
    all_section_outnames="${all_section_outnames} ${sectionoutname}"
    i+=1
done


# plot the graph
python3 graph/graph.py $all_outnames
