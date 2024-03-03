#!/usr/bin/env bash
##########

RUN_P="./build/apps/single -f 0 0.001 0.001"
RUN_I="./build/apps/single -f 1 0.001 0.001"
RUN_S="./build/apps/single -f 2 0.001 0.001"

LIST="0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f single_pdg.data
rm -f single_iso.data
rm -f single_point.data

{
echo "# Created by 'discrete/single'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# ERROR:  off"
echo "# FLUX:   0"
} >> single_pdg.data

{
echo "# Created by 'discrete/single'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# ERROR:  off"
echo "# FLUX:   1"
} >> single_iso.data

{
echo "# Created by 'discrete/single'"
echo "# Parameters"
echo "# Dphi:   0.001rad"
echo "# Dtheta: 0.001rad"
echo "# ERROR:  off"
echo "# FLUX:   2"
} >> single_point.data

for th in $LIST; do
    val=$($RUN_P $th | awk '{ print $3}')
    echo "$th, ${val}" >> single_pdg.data
done

for th in $LIST; do
    val=$($RUN_I $th | awk '{ print $3}')
    echo "$th, ${val}" >> single_iso.data
done

for th in $LIST; do
    val=$($RUN_S $th | awk '{ print $3}')
    echo "$th, ${val}" >> single_point.data
done
