#include <iostream>       // For cout - printing to screen
#include <omp.h>          // For OpenMP - parallel programming
using namespace std;      // Avoid writing std:: everywhere

void parallelBubbleSort(int arr[], int n) {
    // Repeat n times to ensure full sorting in worst case
    for (int i = 0; i < n; i++) {

        // EVEN PHASE: compare pairs (0,1), (2,3), (4,5)...
        #pragma omp parallel for   // Split j-iterations across threads
        for (int j = 0; j < n - 1; j += 2) {  // j jumps by 2 → even indices
            if (arr[j] > arr[j + 1]) {          // If left > right
                swap(arr[j], arr[j + 1]);        // Swap them
            }
        }

        // ODD PHASE: compare pairs (1,2), (3,4), (5,6)...
        #pragma omp parallel for   // Again, split across threads
        for (int j = 1; j < n - 1; j += 2) {  // j starts at 1, jumps by 2
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);  // ✅ FIX: use vector instead of int L[n1], R[n2]

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);

            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

int main() {
    int arr[] = {9, 5, 1, 4, 3, 8, 2};         // Unsorted input array
    int n = sizeof(arr) / sizeof(arr[0]);        // Calculate array size = 7

    cout << "Before Sorting: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";                   // Print original array

    parallelBubbleSort(arr, n);                  // Sort the array in parallel

    cout << "\nAfter Sorting: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";                   // Print sorted array

    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, n - 1);
    }

    cout << "\nAfter Sorting: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    return 0;
}
