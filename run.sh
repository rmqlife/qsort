#!/bin/bash
array=(200 500 1000 2000 5000 10000 20000 50000 100000 200000 500000 1000000 2000000 5000000 10000000)

for j in "${array[@]}"
do
    echo $j
    python gen_data.py $j 1 > input.txt
    gcc qsort1.c -o qsort1 -std=c99 -lm -fopenmp -O3  && ./qsort1 input.txt 0
done


