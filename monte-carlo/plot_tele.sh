#!/usr/bin/env bash
##########

# Detector dimension: 10x10 cm
DET="-l 0.1 -w 0.1"

RUN_P="./cprog/build/apps/tele -f 0 ${DET}"
RUN_I="./cprog/build/apps/tele -f 1 ${DET}"
#RUN_S="./cprog/build/apps/tele -f 2 ${DET}"

LIST="0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5"

rm -f mc_tele_pdg_r.data
rm -f mc_tele_pdg_hz.data
rm -f mc_tele_iso_r.data
rm -f mc_tele_iso_hz.data
#rm -f mc_tele_point.data

{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m\n"
} >> mc_tele_pdg_r.data

{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# FLUX:   0"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m\n"
} >> mc_tele_pdg_hz.data

for th in $LIST; do
    lines=$($RUN_P $th)
    val=$(echo ${lines} | awk '{ print $4}')
    err=$(echo ${lines} | awk '{ print $6}')
    echo "$th ${val} ${err}" >> mc_tele_pdg_r.data
    val=$(echo ${lines} | awk '{ print $17}')
    err=$(echo ${lines} | awk '{ print $20}')
    echo "$th ${val} ${err}" >> mc_tele_pdg_hz.data
done

{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# FLUX:   1"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m\n"
} >> mc_tele_iso_r.data

{
echo "# Created by 'monte-carlo/tele'"
echo "# Parameters"
echo "# FLUX:   1"
echo "# LENGTH: 0.1m"
echo "# WIDTH:  0.1m\n"
} >> mc_tele_iso_hz.data

for th in $LIST; do
    lines=$($RUN_I $th)
    val=$(echo ${lines} | awk '{ print $4}')
    err=$(echo ${lines} | awk '{ print $6}')
    echo "$th ${val} ${err}" >> mc_tele_iso_r.data
    val=$(echo ${lines} | awk '{ print $17}')
    err=$(echo ${lines} | awk '{ print $20}')
    echo "$th ${val} ${err}" >> mc_tele_iso_hz.data
done

#for th in $LIST; do
#    line1=$($RUN_S $th | tail -n 1)
#    val=$(echo ${line1} | awk '{ print $2}')
#    err=$(echo ${line1} | awk '{ print $4}')
#    echo "$th, ${val}" >> mc_tele_point.data
#done
