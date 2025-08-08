#include <bits/stdc++.h>
using namespace std;

// Partition function using pivot at low index
int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= pivot && i <= high - 1) i++;
        while (arr[j] > pivot && j >= low + 1) j--;
        if (i < j) swap(arr[i], arr[j]);
    }
    swap(arr[low], arr[j]);
    return j;
}

// Recursive QuickSort
void qs(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pIndex = partition(arr, low, high);
        qs(arr, low, pIndex - 1);
        qs(arr, pIndex + 1, high);
    }
}

// QuickSort wrapper
vector<int> quickSort(vector<int> arr) {
    qs(arr, 0, arr.size() - 1);
    return arr;
}

int main() {

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    char choice;
    cout << "Do you want to auto-generate random values? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 1000000; //10e6
        }
    } else {
        cout << "Enter " << n << " elements:\n";
        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }
    }

    if (n <= 20) { 
        cout << "Before sorting:\n";
        for (int val : arr) cout << val << " ";
        cout << "\n";
    }

    // Sorting
    arr = quickSort(arr);

    if (n <= 20) {
        cout << "After sorting:\n";
        for (int val : arr) cout << val << " ";
        cout << "\n";
    }

    
    if (n > 20) {
        bool sorted = is_sorted(arr.begin(), arr.end());
        cout << "Array sorted correctly? " << (sorted ? "YES" : "NO") << "\n";
    }

    return 0;
}
