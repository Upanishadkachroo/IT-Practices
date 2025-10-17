#include <bits/stdc++.h>
using namespace std;

int THRESHOLD = 10; // can tune this value

struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
    long long recursiveCalls = 0;
} metrics;

void swapCount(int &a, int &b) {
    swap(a, b);
    metrics.swaps++;
}

void insertionSort(vector<int>& arr, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            metrics.comparisons++;
            arr[j + 1] = arr[j];
            metrics.swaps++;
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        metrics.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swapCount(arr[i], arr[j]);
        }
    }
    swapCount(arr[i + 1], arr[high]);
    return i + 1;
}

void hybridQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int size = high - low + 1;
        if (size <= THRESHOLD) {
            insertionSort(arr, low, high);
            return;
        }
        metrics.recursiveCalls++;
        int pi = partition(arr, low, high);
        hybridQuickSort(arr, low, pi - 1);
        hybridQuickSort(arr, pi + 1, high);
    }
}

void runExperiment(vector<int> arr, const string& name) {
    metrics = {};
    clock_t start = clock();
    hybridQuickSort(arr, 0, arr.size() - 1);
    double duration = double(clock() - start) / CLOCKS_PER_SEC;
    cout << "\n--- " << name << " ---\n";
    cout << "Threshold: " << THRESHOLD << "\n";
    cout << "Comparisons: " << metrics.comparisons << "\n";
    cout << "Swaps: " << metrics.swaps << "\n";
    cout << "Recursive Calls: " << metrics.recursiveCalls << "\n";
    cout << "Time: " << duration << " sec\n";
}

int main() {
    srand(time(0));
    const int N = 10000;

    // Create various input distributions
    vector<int> randomArr(N);
    iota(randomArr.begin(), randomArr.end(), 0);
    shuffle(randomArr.begin(), randomArr.end(), mt19937(random_device()()));

    vector<int> sortedArr = randomArr;
    sort(sortedArr.begin(), sortedArr.end());

    vector<int> reverseArr = sortedArr;
    reverse(reverseArr.begin(), reverseArr.end());

    vector<int> fewUniqueArr(N);
    for (int i = 0; i < N; i++) fewUniqueArr[i] = rand() % 5;

    cout << "===== Hybrid QuickSort + Insertion Sort =====\n";
    runExperiment(randomArr, "Random Input");
    runExperiment(sortedArr, "Sorted Input");
    runExperiment(reverseArr, "Reverse Sorted Input");
    runExperiment(fewUniqueArr, "Few Unique Elements");
}
