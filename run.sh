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
offset=0
while getopts "gn:s:o:" arg; do
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
        o)
            offset=${OPTARG}
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
i=$((offset + 1))
n=$((n + offset))
echo $n
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

    i+=1
done
