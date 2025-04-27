#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace std::chrono;

/*************************
 *  基本排序演算法實作   *
 *************************/
// 插入排序
void insertionSort(vector<int>& a, int l, int r) {
    for (int i = l + 1; i <= r; ++i) {
        int key = a[i];
        int j   = i - 1;
        while (j >= l && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}
// 快速排序（median‑of‑three）
int medianOfThree(vector<int>& a, int l, int r) {
    int m = l + (r - l) / 2;
    if (a[r] < a[l])  swap(a[l], a[r]);
    if (a[m] < a[l])  swap(a[m], a[l]);
    if (a[r] < a[m])  swap(a[r], a[m]);
    swap(a[m], a[r - 1]);               // pivot 放 r‑1
    return a[r - 1];
}
int partition(vector<int>& a, int l, int r) {
    int pivot = medianOfThree(a, l, r);
    int i = l, j = r - 1;
    while (true) {
        while (a[++i] < pivot);
        while (pivot < a[--j]);
        if (i < j) swap(a[i], a[j]);
        else break;
    }
    swap(a[i], a[r - 1]);
    return i;
}
void quickSort(vector<int>& a, int l, int r) {
    if (l + 10 <= r) {
        int p = partition(a, l, r);
        quickSort(a, l, p - 1);
        quickSort(a, p + 1, r);
    } else {
        insertionSort(a, l, r);
    }
}
// 合併排序（迭代）
void merge(vector<int>& a, int l, int m, int r) {
    vector<int> tmp(a.begin() + l, a.begin() + r + 1);
    int i = 0, j = m - l + 1, k = l;
    while (i <= m - l && j <= r - l)
        a[k++] = (tmp[i] <= tmp[j]) ? tmp[i++] : tmp[j++];
    while (i <= m - l) a[k++] = tmp[i++];
    while (j <= r - l) a[k++] = tmp[j++];
}
void iterativeMergeSort(vector<int>& a, int l, int r) {
    int n = r - l + 1;
    for (int sz = 1; sz < n; sz *= 2)
        for (int i = l; i < r; i += 2 * sz)
            merge(a, i, min(i + sz - 1, r), min(i + 2 * sz - 1, r));
}
// 堆排序
void heapify(vector<int>& h, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && h[l] > h[largest]) largest = l;
    if (r < n && h[r] > h[largest]) largest = r;
    if (largest != i) { swap(h[i], h[largest]); heapify(h, n, largest); }
}
void heapSort(vector<int>& a, int l, int r) {
    int n = r - l + 1;
    vector<int> tmp(a.begin() + l, a.begin() + r + 1);
    for (int i = n / 2 - 1; i >= 0; --i) heapify(tmp, n, i);
    for (int i = n - 1; i > 0; --i) { swap(tmp[0], tmp[i]); heapify(tmp, i, 0); }
    copy(tmp.begin(), tmp.end(), a.begin() + l);
}

//測試工具函式
void randomPerm(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) a[i] = i + 1;
    for (int i = n - 1; i > 0; --i) swap(a[i], a[rand() % (i + 1)]);
}

using SortFunc = void(*)(vector<int>&, int, int);
struct Stat { double avg; double worst; };

Stat benchmark(SortFunc f, int n, int trials) {
    long long sumUs = 0, worstUs = 0;
    vector<int> origin(n), work(n);
    for (int t = 0; t < trials; ++t) {
        randomPerm(origin);
        work = origin;
        auto s = high_resolution_clock::now();
        f(work, 0, n - 1);
        auto e = high_resolution_clock::now();
        long long us = duration_cast<microseconds>(e - s).count();
        sumUs  += us;
        worstUs = max(worstUs, us);
    }
    return { sumUs / 1e6 / trials, worstUs / 1e6 }; // 以秒回傳
}

//主程式
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(static_cast<unsigned>(time(nullptr)));

    const int trials = 10;                                   // 重複次數
    const int sizes[] = {500, 1000, 2000, 3000, 4000, 5000}; // 固定 n

    struct Algo { const char* name; SortFunc fn; } algos[] = {
        {"Insertion", insertionSort},
        {"Quick",     quickSort    },
        {"Merge",     iterativeMergeSort},
        {"Heap",      heapSort    }
    };

    cout << left << setw(8)  << "Size" << setw(12) << "Algo"
         << right << setw(14) << "Average(s)" << setw(14) << "Worst(s)" << '\n';

    for (int n : sizes) {
        for (auto &a : algos) {
            Stat st = benchmark(a.fn, n, trials);
            cout << left << setw(8) << n
                 << setw(12) << a.name
                 << right << setw(14) << fixed << setprecision(6) << st.avg
                 << setw(14) << st.worst << '\n';
        }
    }
    return 0;
}
