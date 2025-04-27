#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
using namespace std;

// 插入排序
void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 快速排序
int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[right] < arr[left]) swap(arr[left], arr[right]);
    if (arr[mid] < arr[left]) swap(arr[mid], arr[left]);
    if (arr[right] < arr[mid]) swap(arr[right], arr[mid]);
    swap(arr[mid], arr[right - 1]);
    return arr[right - 1];
}

int partition(vector<int>& arr, int left, int right) {
    int pivot = medianOfThree(arr, left, right);
    int i = left, j = right - 1;
    while (true) {
        while (arr[++i] < pivot);
        while (pivot < arr[--j]);
        if (i < j) swap(arr[i], arr[j]);
        else break;
    }
    swap(arr[i], arr[right - 1]);
    return i;
}

void quickSort(vector<int>& arr, int left, int right) {
    if (left + 10 <= right) {
        int pivotIndex = partition(arr, left, right);
        quickSort(arr, left, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, right);
    } else insertionSort(arr, left, right);
}

// 合併排序
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp(arr.begin() + l, arr.begin() + r + 1);
    int i = 0, j = m - l + 1, k = l;

    while (i <= m - l && j <= r - l)
        arr[k++] = (temp[i] <= temp[j]) ? temp[i++] : temp[j++];
    while (i <= m - l) arr[k++] = temp[i++];
    while (j <= r - l) arr[k++] = temp[j++];
}

void iterativeMergeSort(vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    for (int sz = 1; sz < n; sz *= 2)
        for (int l = left; l < right; l += 2 * sz)
            merge(arr, l, min(l + sz - 1, right), min(l + 2 * sz - 1, right));
}

// 堆排序
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    vector<int> temp(arr.begin() + left, arr.begin() + right + 1);

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(temp, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(temp[0], temp[i]);
        heapify(temp, i, 0);
    }

    copy(temp.begin(), temp.end(), arr.begin() + left);
}

void permute(vector<int>& arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = i + 1;
    for (int i = n - 1; i > 0; i--) swap(arr[i], arr[rand() % (i + 1)]);
}

void generateInsertionWorstCase(vector<int>& arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = n - i;
}

void generateMergeWorstCase(vector<int>& arr, int n) {
    generateInsertionWorstCase(arr, n);
}

double testSort(void (*sortFunc)(vector<int>&, int, int), int n, void (*generateData)(vector<int>&, int), int trials, bool worstCase) {
    double totalTime = 0;
    vector<int> original(n), copy(n);

    for (int t = 0; t < trials; t++) {
        if (generateData && worstCase) generateData(original, n);
        else permute(original, n);

        copy = original;
        clock_t start = clock();
        sortFunc(copy, 0, n - 1);
        clock_t end = clock();
        totalTime += double(end - start) / CLOCKS_PER_SEC;
    }
    return totalTime / trials;
}

int main() {
    srand(time(0));
    int sizes[] = {500, 1000, 2000, 3000, 4000, 5000};
    int trials = 3;

    cout << setw(6) << "n" << setw(12) << "Insertion" << setw(12) << "Quick" << setw(12) << "Merge" << setw(12) << "Heap" << endl;
    for (int n : sizes) {
        cout << setw(6) << n
             << setw(12) << fixed << setprecision(6) << testSort(insertionSort, n, generateInsertionWorstCase, trials, true)
             << setw(12) << testSort(quickSort, n, nullptr, trials, false)
             << setw(12) << testSort(iterativeMergeSort, n, generateMergeWorstCase, trials, true)
             << setw(12) << testSort(heapSort, n, nullptr, trials, false)
             << endl;
    }

    return 0;
}
