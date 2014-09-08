#!/bin/bash
# Authors: Suresh Golconda
#
# Description:script to run program to generate longest path on 32x32 grid.
#
# Arguments: Takes 4 arguments, start-xy, end-xy. 
#
# Output: Re-directs screen output to a file in 'results' folder
#
# Usage: used by other script to run over auto-generated arguments. To
# run manually use following example: 
# $echo "0,0 2,3" | run_knight_long_path.sh

../bin/knight_long_path -a -s $1,$2 -e $3,$4 > results/output_"$1"_"$2"_"$3"_"$4"
