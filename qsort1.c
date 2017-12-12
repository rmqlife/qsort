#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <omp.h>

long STOP_THRESH=10000;
//swap two elements
void swap(double* a, double* b)
{
    double t = *a;
    *a = *b;
    *b = t;
}
 
//  all smaller to left of pivot
//  all greater elements to right of pivot
long partition (double *arr, long low, long high)
{
    long pivot = arr[high];    // pivot
    long i = (low - 1);  // Index of smaller element
    for (long j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

long partition1(double *a, long p, long r)
{
    double lt[r-p];
    double gt[r-p];
    long i;
    long j;
    double key = a[r];
    long lt_n = 0;
    long gt_n = 0;

    #pragma omp parallel for
    for(i = p; i < r; i++){
        if(a[i] < a[r]){
            lt[lt_n++] = a[i];
        }else{
            gt[gt_n++] = a[i];
        }   
    }
    for(i = 0; i < lt_n; i++){
        a[p + i] = lt[i];
    } 
    a[p + lt_n] = key;
    for(j = 0; j < gt_n; j++){
        a[p + lt_n + j + 1] = gt[j];
    }
    return p + lt_n;
} 





 
// The main function that implements QuickSort
void qsort0(double *arr, long low, long high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        long pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        qsort0(arr, low, pi - 1);
        qsort0(arr, pi + 1, high);
    }
}
 

void qsort1(double *arr, long low, long high)
{
    if (low < high){
        /* pi is partitioning index, arr[p] is now
           at right place */
        
        if (high - low<STOP_THRESH) {
            long pi = partition(arr, low, high);
            qsort0(arr, low, pi-1);
            qsort0(arr, pi+1, high);
        }else{
            long pi = partition1(arr, low, high);
			#pragma omp task
            qsort1(arr, low, pi - 1);
			#pragma omp task
            qsort1(arr, pi + 1, high);
			#pragma omp taskwait
            
        }
    }
}


/* Function to print an array */
void printArray(double *arr, long size)
{
    long i;
    for (i=0; i < size; i++)
        printf("%f ", arr[i]);
    printf("n");
}

int main(int argc, char *argv[]){
	if(argc<=2) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
    } 
    int show_fag=0;
   	// read from an array
    FILE * file;
	file = fopen(argv[1] , "r");
	

	if (file) {
	    double *data;
		int num;
		double begin,end;
		int ret = fscanf(file,"%d",&num);
	    data = (double *)malloc(num * sizeof(double));
	    long i;
		for (i=0; i<num; ++i){
			ret = fscanf(file,"%lf",&data[i]);
		}
		begin = omp_get_wtime();
		qsort0(data,0,num-1);
		end = omp_get_wtime();
		if (atoi(argv[2])==1)
			printArray(data,num);
		double time_spent = (double)(end - begin);
		printf("%f\n", time_spent);
	    fclose(file);
	    free(data);
	}
	
	file = fopen(argv[1] , "r");
	if (file) {
	    double *data;
		int num;
		double begin,end;
		int ret = fscanf(file,"%d",&num);
	    data = (double *)malloc(num * sizeof(double));
	    long i;
		for (i=0; i<num; ++i){
			ret = fscanf(file,"%lf",&data[i]);
		}
		STOP_THRESH = 5000;
		begin = omp_get_wtime();
		#pragma omp parallel num_threads(4)
	    {	
		    #pragma omp single nowait
		    {
		        qsort1(data,0,num-1);
		    }
		}
		end = omp_get_wtime();
		if (atoi(argv[2])==1)
			printArray(data,num);
		double time_spent = (double)(end - begin);
		printf("%f\n", time_spent);
	    fclose(file);
	    free(data);
	}
    return 0;
}
