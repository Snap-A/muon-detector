#!/usr/bin/env bash
##########

# Detector dimension: 20x10 cm
DET="-l 0.2 -w 0.1"
SHAPE="-f 0"
ANGLE="0.0"

RUN_P="./cprog/build/apps/tele ${SHAPE} ${DET} ${ANGLE} -s "

LIST="0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f mc_tele_separation_20_hz.data
{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# ANGLE:  ${ANGLE}rad"
echo "# FLUX:   0"
echo "# LENGTH: 0.2m"
echo "# TOTAL:  1000000"
echo "# WIDTH:  0.1m"
} >> mc_tele_separation_20_hz.data

for sep in $LIST; do
    line1=$($RUN_P $sep | tail -n 1)
    val=$(echo ${line1} | awk '{ print $5}')
    err=$(echo ${line1} | awk '{ print $8}')
    echo "$sep ${val} ${err}" >> mc_tele_separation_20_hz.data
done
