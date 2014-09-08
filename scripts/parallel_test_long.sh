#!/bin/bash
# Authors: Suresh Golconda
#
# Description: Using available number of parallel cores to initiate
# program to search for longest path on a 32x32 grid, for given start and end points
#
# Usage: ./parallel_test_long.sh

# Compute number of cores present on the system
last_core_label=`cat /proc/cpuinfo | awk '/^processor/{print $3}'| tail -1`
num_cores=$((last_core_label+1))

echo "Your system has $num_cores cores."
echo "Description: The script will search for longest path on 32x32 grid for different combinations of start and end nodes/cells."
echo "Output: Results are being stored in 'results' sub folder."
echo "Output file format: output_<start-x>_<start-y>_<end-x>_<end-y>"
echo 
echo "Using $num_cores cores. This process may take some time ....."



# Create results folder. Argument "-p" to not complain if folder already exist
mkdir -p results

# run using parallel cores
./generate_arguments.sh | xargs -n 4 -P $num_cores  ./run_knight_long_path.sh