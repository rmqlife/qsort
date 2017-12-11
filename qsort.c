#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <omp.h>
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
 
// The main function that implements QuickSort
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
 

void quickSort1(int arr[], int low, int high)
{
    if (low < high){
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
        // Separately sort elements
        #pragma omp parallel sections 
        {
        	#pragma omp section 
        	{
        		quickSort(arr, low, pi - 1);
        	}
        	#pragma omp section
            {
        		quickSort(arr, pi + 1, high);
        	}
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
	if(argc<=0) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
    } 
    int show_fag=0;
   	// read from an array
    FILE * file;
	file = fopen(argv[1] , "r");
	int num;
	double begin,end;
	if (file) {
		fscanf(file,"%d",&num);
		int data[num];
		for (int i=0; i<num; ++i){
			fscanf(file,"%d",&data[i]);
		}


		//printArray(data,num);
		begin = omp_get_wtime();
		quickSort(data,0,num-1);
		end = omp_get_wtime();
		double time_spent = (double)(end - begin);
		printf("\n%f\n", time_spent);
		//printArray(data,num);
	    fclose(file);
	    
	}
    return 0;
}
