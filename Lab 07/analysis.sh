#!/bin/bash
echo Analysing and Generating PNG!!

# Different Sizes of Cache
SIZE='
2
4
5
8
16
32
64
128
256
512
1024
'

PROGRAM='addmillion.c';

# Compile the program
gcc ${PROGRAM} -lpthread -o a.out

# Create and clear the file to store the results of the analysis
touch results.txt
> results.txt

# Run the analysis for different sizes of threads
for i in ${SIZE}; do
    echo -n "${i} " >> results.txt
    ./a.out ${i} | grep "Time spent:" | cut -d ' ' -f 3 >> results.txt 
done

# Plot the results
cat results.txt | graph -T png --bitmap-size "1400x1400" -g 3 -L "Threads vs Time" -X "Number of Threads" -Y "Time Taken (in ms)" -r 0.25> ./plot.png

rm results.txt

echo Done!!
