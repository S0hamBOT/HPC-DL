#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Print Array
void printArray(vector<int> &arr) {
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

int main() {

    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        cout << "Element " << i + 1 << ": ";
        cin >> arr[i];
    }

    cout << "\nOriginal Array:\n";
    printArray(arr);
    cout << endl;

    double start, end;

    // ================= SEQUENTIAL =================
    int min_seq = arr[0];
    int max_seq = arr[0];
    long long sum_seq = 0;

    start = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_seq) min_seq = arr[i];
        if (arr[i] > max_seq) max_seq = arr[i];
        sum_seq += arr[i];
    }
    double avg_seq = (double)sum_seq / n;
    end = omp_get_wtime();

    cout << "----- Sequential Results -----\n";
    cout << "Min: " << min_seq << endl;
    cout << "Max: " << max_seq << endl;
    cout << "Sum: " << sum_seq << endl;
    cout << "Average: " << avg_seq << endl;
    cout << "Time: " << end - start << " seconds\n\n";

    // ================= PARALLEL =================
    int min_par = arr[0];
    int max_par = arr[0];
    long long sum_par = 0;

    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_par) reduction(min:min_par) reduction(max:max_par)
    for (int i = 0; i < n; i++) {
        sum_par += arr[i];
        if (arr[i] < min_par) min_par = arr[i];
        if (arr[i] > max_par) max_par = arr[i];
    }
    double avg_par = (double)sum_par / n;
    end = omp_get_wtime();

    cout << "----- Parallel Results -----\n";
    cout << "Min: " << min_par << endl;
    cout << "Max: " << max_par << endl;
    cout << "Sum: " << sum_par << endl;
    cout << "Average: " << avg_par << endl;
    cout << "Time: " << end - start << " seconds\n";

    return 0;
}
