#!/usr/bin/env bash
##########

# Detector dimension: 10x10 cm, separation 30 cm
DET="-l 0.1 -w 0.1 -s 0.3"

RUN_P="./build/apps/tele -f 0 ${DET} 0.001 0.001 0.005 0.005"

LIST="0.0 0.5236 1.0472 1.5708"

rm -f tele_pdg_exp_s0.3.data

{
echo "# Created by 'discrete/tele'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# Dx:     0.005m"
echo "# Dy:     0.005m"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# SEP:    0.3m"
echo "# WIDTH:  0.1m"
} >> tele_pdg_exp_s0.3.data

for th in $LIST; do
    val=$($RUN_P $th | tail -n 1 | awk '{ print $4}')
    echo "$th, ${val}" >> tele_pdg_exp_s0.3.data
done
