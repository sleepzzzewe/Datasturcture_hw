# 41243220 41243236
***
#### 作業一
## 解題說明
***
**本題要求不同排序法之間的效能差異、最壞情況、平均值**

### 解題策略
#### 1.**資料產生：使用 Fisher–Yates 洗牌 (randomPerm) 生成隨機排列**
    ```cpp
    void randomPerm(vector<int>& a) {
        int n = a.size();
        for (int i = 0; i < n; ++i) a[i] = i + 1;
        for (int i = n - 1; i > 0; --i) swap(a[i], a[rand() % (i + 1)]);
    }
    ```
#### 2.**排序核心：**

**插入排序**：連續掃描並插入，適合小區間

**快速排序**：三數取中 pivot + cutoff = 10，遞迴深度平均 O(log n)

**合併排序**：自底向上 (iterative) 實作以減少遞迴成本

**堆排序**：先建最大堆再反覆 extract‑max

**計時**：std::chrono::high_resolution_clock 量測微秒，記錄總和與最大值

## 程式實作
***
**benchmark()**：多接一個產生器參數，用來決定輸入形態

genFunc == nullptr   呼叫 randomPerm() → 平均情況
傳入自訂函式         產生極端輸入       → 最壞情況

using DataGen = void()(vector<int>&);   // ← 新增型別別名

    Stat benchmark(Sort sortFn, DataGen genFunc,
                   int n, int trials)
    {
        long long totalUs = 0, worstUs = 0;
        vector<int> src(n), work(n);

    for (int t = 0; t < trials; ++t) {
        (genFunc ? genFunc : randomPerm)(src);     // ★ 決定要不要洗牌
        work = src;

        auto s = high_resolution_clock::now();
        sortFn(work, 0, n - 1);
        auto e = high_resolution_clock::now();

        long long us = duration_cast<microseconds>(e - s).count();
        totalUs += us;
        worstUs  = max(worstUs, us);

        if (!is_sorted(work.begin(), work.end())) {   // 正確性
            cerr << "Sort failed!\n"; exit(1);
        }
    }
    return { totalUs / 1e6 / trials, worstUs / 1e6 }; // 轉秒
    }

### 三個最壞輸入產生器範例
    void desc(vector<int>& a) {             // 插入、Merge 最差
        int n = a.size();
        for (int i = 0; i < n; ++i) a[i] = n - i;
    }
    void asc(vector<int>& a)  {             // Quick(第一元素 pivot) 最差
        int n = a.size();
        for (int i = 0; i < n; ++i) a[i] = i + 1;
    }
    void almostSorted(vector<int>& a) {     // 模擬「接近已排好」情境
        asc(a);
        if (a.size() > 1) swap(a[0], a.back());
    }
## 效能分析
***
| 演算法 | 時間複雜度 – 平均 | 時間複雜度 – 最壞 | 空間複雜度 |
|--------|------------------|-------------------|-----------|
| Insertion Sort | O(n²) | O(n²) | O(1) |
| Quick Sort<br>(median-of-three) | O(n log n) | O(n log n) <br>(pivot 改良後，最壞已降低) | O(log n) 遞迴堆疊 |
| Merge Sort<br>(iterative) | O(n log n) | O(n log n) | O(n) 暫存陣列 |
| Heap Sort | O(n log n) | O(n log n) | O(1) 另配堆陣列 |

**推導備註
Insertion：內外雙迴圈最多比較 \(n(n-1)/2\) 次 → Θ(n²)。
Quick：三數取中讓遞迴樹深度 ≈ log n，每層處理 n → Θ(n log n)。
Merge：拆分 log n 層，每層合併 n → Θ(n log n)。
Heap：建堆 Θ(n) + n 次下濾 Θ(log n) → Θ(n log n)；空間僅常數暫存。**
## 測試與驗證
***
### 程式結果：
| Size  | Algo      | Average(s) | Worst(s) |
|-------|-----------|------------|----------|
| 500   | Insertion | 0.000299   | 0.000998 |
| 500   | Quick     | 0.000000   | 0.000000 |
| 500   | Merge     | 0.000199   | 0.000997 |
| 500   | Heap      | 0.000100   | 0.001000 |
| 1000  | Insertion | 0.000897   | 0.001000 |
| 1000  | Quick     | 0.000000   | 0.000000 |
| 1000  | Merge     | 0.000299   | 0.000998 |
| 1000  | Heap      | 0.000200   | 0.001000 |
| 2000  | Insertion | 0.003391   | 0.003989 |
| 2000  | Quick     | 0.000100   | 0.001000 |
| 2000  | Merge     | 0.000200   | 0.000999 |
| 2000  | Heap      | 0.000498   | 0.001000 |
| 3000  | Insertion | 0.007874   | 0.008969 |
### 最壞執行時間表格
![image](https://github.com/sleepzzzewe/Datasturcture_hw/blob/main/homework1/src/worst_time.png)
### 平均時間表格
![image](https://github.com/sleepzzzewe/Datasturcture_hw/blob/main/homework1/src/avg_time.png)
## 申論及開發報告
***
### 1.選擇的排序法
**插入排序**：用於小型資料集。其時間複雜度為 O(n²)，適用於數據量較小或已部分排序的情況。
    
**快速排序**：一種分治策略的排序算法，其時間複雜度通常為 O(n log n)，在許多情況下表現較好。選擇了「中位數選擇」方法（Median-of-three）來選擇 pivot 元素，改善性能。
    
**合併排序**：也是一種分治策略，時間複雜度為 O(n log n)，但需要額外的空間來進行合併操作。在這個實作中，選擇了迭代版本的合併排序，避免了遞迴可能帶來的堆疊溢出問題。
        
**堆排序**：堆排序利用了堆這一資料結構，時間複雜度為 O(n log n)，並且不需要額外的空間，適合於大資料集。

### 2.測試工具
我們設計了一個基準測試工具 benchmark()，其會測量每個排序算法在不同資料集大小下的平均執行時間和最差執行時間。測試過程中，對於每一個資料集，我們會隨機打亂數據，並執行每個算法多次，計算其執行時間的平均值和最差值。

### 3.性能比較與分析
測試結果會顯示每種排序算法在不同大小資料集下的表現。從時間複雜度的理論推斷，快速排序、合併排序和堆排序的性能會比插入排序更好，尤其是在資料集較大時，插入排序的 O(n²) 時間複雜度將導致其表現顯著下降。
    
此外，合併排序和堆排序的性能應該相對穩定，並且都能夠在較大的資料集下保持較好的表現。而快速排序則可能因為最差情況的 O(n²) 時間複雜度而表現不穩定，但在多數隨機資料情況下，快速排序往往表現最好。

### 4.選擇排序算法的理由
**插入排序**：適用於小型資料集或近似排序的情況。其簡單易實作，且能夠在極小的資料集上表現出色。
    
**快速排序**：是一個非常高效的排序算法，對於大多數情況下的隨機資料集，快速排序表現最佳。選擇使用 median-of-three 方法來選擇 pivot，這有助於減少快速排序的最差情況。
   
**合併排序**：適用於資料量極大的情況，尤其在資料集必須保持穩定排序的場合中表現突出。這也是為何選擇使用迭代版而非遞迴版的合併排序，減少遞迴帶來的堆疊溢出問題。
    
**堆排序**：雖然堆排序的常數因子較高，但其不需要額外空間，並且對於極大的資料集有著穩定的表現。

## 結論
***
透過排序算法的比較，程式展示了不同排序算法的性能差異。每種排序算法都有其適用的情境，選擇合適的排序算法可以提高程式效率。可以考慮進一步優化測試過程，增加更多的測試案例，並針對不同的資料特徵進行測試。

