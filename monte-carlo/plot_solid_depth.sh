#!/usr/bin/env bash
##########

TOTAL_N=60000000
ORIGIN=30.0       # 30 times area
TRACK_L=0.00001   # 0.01 mm

RUN_P="./build/apps/solid -f 0 -e ${TOTAL_N} -o ${ORIGIN} -t ${TRACK_L} -d "
RUN_I="./build/apps/solid -f 1 -e ${TOTAL_N} -o ${ORIGIN} -t ${TRACK_L} -d "

LIST="0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"
STEPS="0.0001 0.001 0.01 0.03 0.1"

IDX=0
rm -f solid_pdg_idx*.data
rm -f solid_iso_idx*.data

for depth in $STEPS; do
    ((IDX=IDX+1))
    echo "Runnning IDX=${IDX} => DEPTH=${depth}m"

    {
        echo "# Created by 'monte-carlo/solid'"
        echo "# Parameters"
        echo "# BINS:   100"
        echo "# DEPTH:  ${depth}m"
        echo "# FLUX:   0"
        echo "# LENGTH: 0.1m"
        echo "# TOTAL:  ${TOTAL_N}"
        echo "# TRACK:  ${TRACK_L}m"
        echo "# WIDTH:  0.1m"
        echo "# WORLD:  ${ORIGIN}x"
    } >> solid_pdg_idx${IDX}.data

    {
        echo "# Created by 'monte-carlo/solid'"
        echo "# Parameters"
        echo "# BINS:   100"
        echo "# DEPTH:  ${depth}m"
        echo "# FLUX:   1"
        echo "# LENGTH: 0.1m"
        echo "# TOTAL:  ${TOTAL_N}"
        echo "# TRACK:  ${TRACK_L}m"
        echo "# WIDTH:  0.1m"
        echo "# WORLD:  ${ORIGIN}x"
    } >> solid_iso_idx${IDX}.data

    for th in $LIST; do
        line1=$($RUN_P ${depth} ${th} | tail -n 1)
        val=$(echo ${line1} | awk '{ print $5}')
        err=$(echo ${line1} | awk '{ print $8}')
        echo "${th} ${val} ${err}" >> solid_pdg_idx${IDX}.data
    done

    for th in $LIST; do
        line1=$($RUN_I ${depth} ${th} | tail -n 1)
        val=$(echo ${line1} | awk '{ print $5}')
        err=$(echo ${line1} | awk '{ print $8}')
        echo "${th} ${val} ${err}" >> solid_iso_idx${IDX}.data
    done
done
