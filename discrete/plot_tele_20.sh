#!/usr/bin/env bash
##########

# Detector dimension: 20x10 cm
DET="-l 0.2 -w 0.1"

RUN_P="./build/apps/tele -f 0 ${DET} 0.001 0.001 0.005 0.005"
RUN_I="./build/apps/tele -f 1 ${DET} 0.001 0.001 0.005 0.005"
RUN_S="./build/apps/tele -f 2 ${DET} 0.001 0.001 0.005 0.005"

LIST="0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f tele_pdg_20.data
rm -f tele_iso_20.data
rm -f tele_point_20.data

{
echo "# Created by 'discrete/tele'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# Dx:     0.005m"
echo "# Dy:     0.005m"
echo "# FLUX:   0"
echo "# LENGTH: 0.2m"
echo "# WIDTH:  0.1m"
} >> tele_pdg.data

{
echo "# Created by 'discrete/tele'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# Dx:     0.005m"
echo "# Dy:     0.005m"
echo "# FLUX:   1"
echo "# LENGTH: 0.2m"
echo "# WIDTH:  0.1m"
} >> tele_iso.data

{
echo "# Created by 'discrete/tele'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# Dx:     0.005m"
echo "# Dy:     0.005m"
echo "# FLUX:   2"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.2m"
} >> tele_point.data

for th in $LIST; do
    val=$($RUN_P $th | tail -n 1 | awk '{ print $4}')
    echo "$th, ${val}" >> tele_pdg_20.data
done

for th in $LIST; do
    val=$($RUN_I $th | tail -n 1 | awk '{ print $4}')
    echo "$th, ${val}" >> tele_iso_20.data
done

for th in $LIST; do
    val=$($RUN_S $th | tail -n 1 | awk '{ print $4}')
    echo "$th, ${val}" >> tele_point_20.data
done
