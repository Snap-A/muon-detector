#!/usr/bin/env bash
##########

RUN_P="./build/apps/solid -f 0 -e 60000000 -o "

LIST="4.0 6.0 8.0 10.0 12.0 16.0 20.0 30.0"

rm -f solid_pdg_world_0.data
rm -f solid_pdg_world_45.data

{
echo "# Created by 'monte-carlo/solid'"
echo "# Parameters"
echo "# BINS:   100"
echo "# DEPTH:  0.01m"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# THETA:  0deg"
echo "# TOTAL:  60000000"
echo "# TRACK:  0.003m"
echo "# WIDTH:  0.1m"
} >> solid_pdg_world_0.data

{
echo "# Created by 'monte-carlo/solid'"
echo "# Parameters"
echo "# BINS:   100"
echo "# DEPTH:  0.01m"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# THETA:  45deg"
echo "# TOTAL:  60000000"
echo "# TRACK:  0.003m"
echo "# WIDTH:  0.1m"
} >> solid_pdg_world_45.data

for wrld in $LIST; do
    line1=$($RUN_P ${wrld} 0 | tail -n 1)
    val=$(echo ${line1} | awk '{ print $5}')
    err=$(echo ${line1} | awk '{ print $8}')
    echo "${wrld} ${val} ${err}" >> solid_pdg_world_0.data
done

for wrld in $LIST; do
    line1=$($RUN_P ${wrld} 0.785 | tail -n 1)
    val=$(echo ${line1} | awk '{ print $5}')
    err=$(echo ${line1} | awk '{ print $8}')
    echo "${wrld} ${val} ${err}" >> solid_pdg_world_45.data
done
