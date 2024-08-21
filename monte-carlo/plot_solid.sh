#!/usr/bin/env bash
##########

RUN_P="./build/apps/solid -f 0 -e 60000000 -o 30.0"
RUN_I="./build/apps/solid -f 1 -e 60000000 -o 30.0"

LIST="0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f solid_pdg.data
rm -f solid_iso.data

{
echo "# Created by 'monte-carlo/solid'"
echo "# Parameters"
echo "# BINS:   100"
echo "# DEPTH:  0.01m"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# TOTAL:  60000000"
echo "# TRACK:  0.003m"
echo "# WIDTH:  0.1m"
echo "# WORLD:  30.0x"
} >> solid_pdg.data

{
echo "# Created by 'monte-carlo/solid'"
echo "# Parameters"
echo "# BINS:   100"
echo "# DEPTH:  0.01m"
echo "# FLUX:   1"
echo "# LENGTH: 0.1m"
echo "# TOTAL:  60000000"
echo "# TRACK:  0.003m"
echo "# WIDTH:  0.1m"
echo "# WORLD:  30.0x"
} >> solid_iso.data

for th in $LIST; do
    line1=$($RUN_P ${th} | tail -n 1)
    val=$(echo ${line1} | awk '{ print $5}')
    err=$(echo ${line1} | awk '{ print $8}')
    vala=$(echo "${val}*100" | bc)
    erra=$(echo "${err}*100" | bc)
    echo "${th} ${vala} ${erra}" >> solid_pdg.data
done

for th in $LIST; do
    line1=$($RUN_I ${th} | tail -n 1)
    val=$(echo ${line1} | awk '{ print $5}')
    err=$(echo ${line1} | awk '{ print $8}')
    vala=$(echo "${val}*100" | bc)
    erra=$(echo "${err}*100" | bc)
    echo "${th} ${vala} ${erra}" >> solid_iso.data
done
