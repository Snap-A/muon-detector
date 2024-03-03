#!/usr/bin/env bash
##########

# Detector dimension: 10x10 cm
DET="-l 0.1 -w 0.1"
SHAPE="-f 0"
ANGLE="0.0"

RUN_P="./build/apps/tele ${SHAPE} 0.001 0.001 0.005 0.005 ${ANGLE} ${DET} -s "

LIST="0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f tele_separation_hz.data

{
echo "# Created by 'discrete/tele'"
echo "# Parameters"
echo "# ANGLE:  ${ANGLE}rad"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# Dx:     0.005m"
echo "# Dy:     0.005m"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m"
} >> tele_separation_hz.data

for sep in $LIST; do
    val=$($RUN_P $sep | tail -n 1 | awk '{ print $4}')
    echo "$sep, ${val}" >> tele_separation_hz.data
done
