#!/bin/bash
# Authors: Suresh Golconda
#
# Description: Generates all possible arguments to run program to
# search for longest path on a 32x32 grid
#
# Usage: This script is NOT meant to be run manually. This script is
# used by other scripts to generate arguments to search algorithm
# program.


max=31

for sx in `seq 0 $max`;
do
    for sy in `seq 0 $max`;
    do
	for ex in `seq 0 $max`;
	do
	    for ey in `seq 0 $max`;
            do

		if [ $sx != $ex -o $sy != $ey ]
		then
		    echo "$sx $sy $ex $ey"

		fi
	    done  
	done
    done
done


