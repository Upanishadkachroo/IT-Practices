#include <bits/stdc++.h>
using namespace std;

enum PivotStrategy { FIRST, RANDOM, MEDIAN3 };

// Global counters
struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
    long long recursiveCalls = 0;
};

Metrics metrics;

// Swap with counting
void swapCount(int &a, int &b) {
    swap(a, b);
    metrics.swaps++;
}

// Partition function
int partition(vector<int> &arr, int low, int high, PivotStrategy strategy) {
    int pivotIndex;

    // --- Pivot selection ---
    if (strategy == FIRST)
        pivotIndex = low;
    else if (strategy == RANDOM)
        pivotIndex = low + rand() % (high - low + 1);
    else { // MEDIAN OF THREE
        int mid = (low + high) / 2;
        int a = arr[low], b = arr[mid], c = arr[high];
        // Find median of (a,b,c)
        if ((a > b) != (a > c)) pivotIndex = low;
        else if ((b > a) != (b > c)) pivotIndex = mid;
        else pivotIndex = high;
    }

    swapCount(arr[pivotIndex], arr[high]);
    int pivot = arr[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        metrics.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swapCount(arr[i], arr[j]);
        }
    }
    swapCount(arr[i + 1], arr[high]);
    return i + 1;
}

// QuickSort recursive function
void quickSort(vector<int> &arr, int low, int high, PivotStrategy strategy) {
    if (low < high) {
        metrics.recursiveCalls++;
        int pi = partition(arr, low, high, strategy);
        quickSort(arr, low, pi - 1, strategy);
        quickSort(arr, pi + 1, high, strategy);
    }
}

void runExperiment(vector<int> arr, PivotStrategy strategy, string name) {
    metrics = {}; // reset
    quickSort(arr, 0, arr.size() - 1, strategy);
    cout << "\n--- " << name << " ---\n";
    cout << "Comparisons: " << metrics.comparisons << "\n";
    cout << "Swaps: " << metrics.swaps << "\n";
    cout << "Recursive Calls: " << metrics.recursiveCalls << "\n";
}

int main() {
    srand(time(0));
    const int N = 10000;

    // --- Different distributions ---
    vector<int> randomArr(N);
    iota(randomArr.begin(), randomArr.end(), 0);
    shuffle(randomArr.begin(), randomArr.end(), std::mt19937(std::random_device()()));

    vector<int> sortedArr = randomArr;
    sort(sortedArr.begin(), sortedArr.end());

    vector<int> reverseArr = sortedArr;
    reverse(reverseArr.begin(), reverseArr.end());

    vector<int> fewUniqueArr(N);
    for (int i = 0; i < N; i++) fewUniqueArr[i] = rand() % 5;

    // --- Run tests ---
    cout << "==== Random Input ====\n";
    runExperiment(randomArr, FIRST, "Pivot: First Element");
    runExperiment(randomArr, RANDOM, "Pivot: Random Element");
    runExperiment(randomArr, MEDIAN3, "Pivot: Median of Three");

    cout << "\n==== Sorted Input ====\n";
    runExperiment(sortedArr, FIRST, "Pivot: First Element");
    runExperiment(sortedArr, RANDOM, "Pivot: Random Element");
    runExperiment(sortedArr, MEDIAN3, "Pivot: Median of Three");

    cout << "\n==== Reverse Sorted Input ====\n";
    runExperiment(reverseArr, FIRST, "Pivot: First Element");
    runExperiment(reverseArr, RANDOM, "Pivot: Random Element");
    runExperiment(reverseArr, MEDIAN3, "Pivot: Median of Three");

    cout << "\n==== Few Unique Elements ====\n";
    runExperiment(fewUniqueArr, FIRST, "Pivot: First Element");
    runExperiment(fewUniqueArr, RANDOM, "Pivot: Random Element");
    runExperiment(fewUniqueArr, MEDIAN3, "Pivot: Median of Three");

    return 0;
}
