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
while getopts "gn:s:" arg; do
    case $arg in
        n)
            n=${OPTARG}
            ;;
        g) 
            args="${args} -g"
            ;;
        s) 
            basename=${OPTARG}
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
    name="${out_dir}/${basename}${i}"
    outname="${name}-out.out"
    sectionoutname="${name}-section.out"

    # run the simulation
    ./build/simulation -n $name $args

    if [ $? -ne "0" ]; then
        exit 1
    fi

    all_outnames="${all_outnames} ${outname}"
    all_section_outnames="${all_section_outnames} ${sectionoutname}"
    i+=1
done


# plot the graph
python3 graph/graph.py $all_outnames
png_name="${basename}.png"
mv influenza.png $png_name
