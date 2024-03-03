#!/usr/bin/env bash
##########

# Detector dimension: 10x10 cm
DET="-l 0.1 -w 0.1"
SHAPE="-f 1"
ANGLE="0.0"

RUN_P="./cprog/build/apps/tele ${SHAPE} ${DET} ${ANGLE} -s "

LIST="0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f mc_tele_separation_iso_r.data
rm -f mc_tele_separation_iso_hz.data

{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# ANGLE:  ${ANGLE}rad"
echo "# FLUX:   1"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m"
} >> mc_tele_separation_iso_r.data

{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# ANGLE:  ${ANGLE}rad"
echo "# FLUX:   1"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m"
} >> mc_tele_separation_iso_hz.data

for sep in $LIST; do
    lines=$($RUN_P $sep)
    val=$(echo ${lines} | awk '{ print $4}')
    err=$(echo ${lines} | awk '{ print $6}')
    echo "$sep ${val} ${err}" >> mc_tele_separation_iso_r.data
    val=$(echo ${lines} | awk '{ print $17}')
    err=$(echo ${lines} | awk '{ print $20}')
    echo "$sep ${val} ${err}" >> mc_tele_separation_iso_hz.data
done
