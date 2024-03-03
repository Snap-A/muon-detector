#!/usr/bin/env bash
##########

RUN_P="./build/apps/single -f 0"
RUN_E="./build/apps/single -e -f 0"

LIST="0.1 0.05 0.02 0.01 0.005 0.002 0.001 0.0005"

rm -f single_zero.data
rm -f single_zero_error.data

{
echo "# Created by 'discrete/single'"
echo "# Parameters"
echo "# ANGLE:  0rad"
echo "# ERROR:  off"
echo "# FLUX:   0"
} >> single_zero.data

{
echo "# Created by 'discrete/single'"
echo "# Parameters"
echo "# ANGLE:  0rad"
echo "# ERROR:  on"
echo "# FLUX:   0"
} >> single_zero_error.data

for del in $LIST; do
    val=$($RUN_P $del $del 0.0 | awk '{ print $3}')
    echo "$del, ${val}" >> single_zero.data
done

for del in $LIST; do
    val=$($RUN_E $del $del 0.0 | awk '{ print $3, $9}')
    echo "$del, ${val}" >> single_zero_error.data
done
