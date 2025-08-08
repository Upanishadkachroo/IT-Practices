#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Partition function
int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= pivot && i <= high - 1) i++;
        while (arr[j] > pivot && j >= low + 1) j--;
        if (i < j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;

    return j;
}

// Recursive QuickSort
void qs(int arr[], int low, int high) {
    if (low < high) {
        int pIndex = partition(arr, low, high);
        qs(arr, low, pIndex - 1);
        qs(arr, pIndex + 1, high);
    }
}

// Function to check if sorted
bool is_sorted(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) return false;
    }
    return true;
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    char choice;
    printf("Do you want to auto-generate random values? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 1000000; // up to 1e6
        }
    } else {
        printf("Enter %d elements:\n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    }

    if (n <= 20) {
        printf("Before sorting:\n");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
    }

    // Sorting
    qs(arr, 0, n - 1);

    if (n <= 20) {
        printf("After sorting:\n");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
    }

    if (n > 20) {
        printf("Array sorted correctly? %s\n", is_sorted(arr, n) ? "YES" : "NO");
    }

    free(arr);
    return 0;
}
