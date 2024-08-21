#!/usr/bin/env bash
##########

TOTAL_N=60000000

RUN_P="./build/apps/solid -f 0 -e ${TOTAL_N} -o 30.0 -p 2"

rm -f solid_pdg_trans_0.data
rm -f trans_0.data

{
echo "# Created by 'monte-carlo/solid'"
echo "# Parameters"
echo "# BINS:   100"
echo "# DEPTH:  0.01m"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# THETA:  0deg"
echo "# TOTAL:  ${TOTAL_N}"
echo "# TRACK:  0.003m"
echo "# WIDTH:  0.1m"
echo "# WORLD:  30.0x"
} >> trans_0.data

$RUN_P 0

cat trans_0.data trans.data > solid_pdg_trans_0.data
