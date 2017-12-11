python gen_data.py 10 1 > input.txt
gcc qsort1.c -o qsort1 -std=c99 -lm -fopenmp -O3  && ./qsort1 input.txt 1

