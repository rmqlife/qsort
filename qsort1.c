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
int partition (double *arr, int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
    for (int j = low; j <= high- 1; j++)
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

int partition1(double * a, int p, int r)
{
    int lt[r-p];
    int gt[r-p];
    int i;
    int j;
    int key = a[r];
    int lt_n = 0;
    int gt_n = 0;

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
void qsort0(double *arr, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        qsort0(arr, low, pi - 1);
        qsort0(arr, pi + 1, high);
    }
}
 

void qsort1(double *arr, int low, int high)
{
    if (low < high){
        /* pi is partitioning index, arr[p] is now
           at right place */
        
        if (high - low<STOP_THRESH) {
            int pi = partition(arr, low, high);
            qsort0(arr, low, pi-1);
            qsort0(arr, pi+1, high);
        }else{
            int pi = partition1(arr, low, high);
			#pragma omp task
            qsort1(arr, low, pi - 1);
			#pragma omp task
            qsort1(arr, pi + 1, high);
			#pragma omp taskwait
            
        }
    }
}


/* Function to print an array */
void printArray(double *arr, int size)
{
    int i;
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
		#pragma omp parallel num_threads(8)
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
