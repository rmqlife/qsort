#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <omp.h>

#define STOP_THRESH 1000
//swap two elements
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
 
//  all smaller to left of pivot
//  all greater elements to right of pivot
int partition (int arr[], int low, int high)
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

#define THREADS_CNT 8
int partition1(int arr[], int low, int high)
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




 
// The main function that implements QuickSort
void qsort0(int *arr, int low, int high)
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
 

void qsort1(int *arr, int low, int high)
{
    if (low < high){
        /* pi is partitioning index, arr[p] is now
           at right place */
        
        if (high - low<STOP_THRESH) {
            int pi = partition(arr, low, high);
            qsort0(arr, low, pi-1);
            qsort0(arr, low, pi+1);
        }else{
            int pi = partition(arr, low, high);
            #pragma omp task
            qsort1(arr, low, pi - 1);
            #pragma omp task
            qsort1(arr, pi + 1, high);
            #pragma omp taskwait
        }
    }
}


/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
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
	    int *data;
		int num;
		double begin,end;
		int ret = fscanf(file,"%d",&num);
	    data = (int *)malloc(num * sizeof(int));
	    long i;
		for (i=0; i<num; ++i){
			ret = fscanf(file,"%d",&data[i]);
		}
		printf("endi %ld", i);
		begin = omp_get_wtime();
		qsort0(data,0,num-1);
		end = omp_get_wtime();
		if (atoi(argv[2])==1)
			printArray(data,num);
		double time_spent = (double)(end - begin);
		printf("\n%f\n", time_spent);
	    fclose(file);
	    free(data);
	}
	
	file = fopen(argv[1] , "r");
	if (file) {
	    int *data;
		int num;
		double begin,end;
		int ret = fscanf(file,"%d",&num);
	    data = (int *)malloc(num * sizeof(int));
	    long i;
		for (i=0; i<num; ++i){
			ret = fscanf(file,"%d",&data[i]);
		}
		printf("endi %ld", i);
		begin = omp_get_wtime();
		qsort1(data,0,num-1);
		end = omp_get_wtime();
		if (atoi(argv[2])==1)
			printArray(data,num);
		double time_spent = (double)(end - begin);
		printf("\n%f\n", time_spent);
	    fclose(file);
	    free(data);
	}
    return 0;
}
