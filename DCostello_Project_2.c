#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Fields
 */
int* unsorted;
int* sorted;
#define ARRAYSZ 20
pthread_t thread1, thread2, thread3;


void allocate_array(int sz) {
    int i;
    // Allocate array
    unsorted = malloc(sz * sizeof(int));

    // Filling array with 0's
    for(i = 0; i < sz; i ++)
        unsorted[i] = rand() % 100 + 1;
        // Algorithm for bounding the rand() function: https://stackoverflow.com/questions/1202687/how-do-i-get-a-specific-range-of-numbers-from-rand
}

void print_array(int* arr, int sz) {
    int i;

    for(int i = 0; i < sz; i++)
        printf("[%d]: %d\n", i, arr[i]);
}

int* build_counting_array() {
    int* counting_array = malloc(100 * sizeof(int));
    int i;

    for(i = 0; i < 100; i++) {
        counting_array[i] = 0;
    }

    return counting_array;
}

void* counting_sort(int* half) {
    int* count_array = build_counting_array();
    int halfIdx = 0; // Index to store where we are in the half
    int i;
    // Perform the counting sort
    for(i = 0; i < ARRAYSZ / 2; i++)
        count_array[half[i]]++; // Add one to the index the value belongs to
    // Set the sorted values to this half of the array
    for(i = 0; i < 100; i++)
        while (count_array[i] != 0) {
            half[halfIdx++] = i;
            count_array[i]--;
        }

    free(count_array);
}

void merge() {
    int i;
    int idx1 = 0; // Index of first half of the array
    int idx2 = ARRAYSZ / 2; // Index of second half
    // Merge the two halves to the array "sorted"
    for(i = 0; i < ARRAYSZ; i++) {
        if(idx1 >= ARRAYSZ / 2) // If we have no more elements in the first half, fill the rest of the 2nd half in
            sorted[i] = unsorted[idx2++];
        else if(idx2 >= ARRAYSZ) // If we have no more elements in the second half, fill the rest of the 1st half in
            sorted[i] = unsorted[idx1++];
        // OTHERWISE: Do a normal operation
        else if(unsorted[idx1] <= unsorted[idx2]) // If the first half's current element >= the second half's, put that in the next available slot in the sorted array
            sorted[i] = unsorted[idx1++];
        else // If the first half's element < second half's, put the second half's element in the next available slot in the sorted array
            sorted[i] = unsorted[idx2++];
    }
}

int main (int argc, char *argv[]) {

    allocate_array(ARRAYSZ); // Now that we know the size, we can allocate the array
    printf("\nUnsorted Array:\n");
    print_array(unsorted, ARRAYSZ);

    // Allocate pointers to both halves of the array
    int* unsorted_half_1 = unsorted;
    int* unsorted_half_2 = unsorted + ARRAYSZ / 2;

    // Run the counting sort on both halves
    pthread_create(&thread1, NULL, (void*)counting_sort, unsorted_half_1);
    pthread_create(&thread2, NULL, (void*)counting_sort, unsorted_half_2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Allocate size for the sorted array
    sorted = malloc(ARRAYSZ * sizeof(int));

    // Merge the two sorted halves
    pthread_create(&thread3, NULL, (void*)merge, NULL);
    pthread_join(thread3, NULL);

    printf("\nSorted Array:\n");
    print_array(sorted, ARRAYSZ);

    return 0;
}