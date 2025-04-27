# 41243220 41243236
***
##### 作業一
### 解題說明
***
**本題要求不同排序法之間的效能差異、最壞情況、平均值**

#### 解題策略
**1.資料產生：使用 Fisher–Yates 洗牌 (randomPerm) 生成隨機排列。**
```cpp
void randomPerm(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) a[i] = i + 1;
    for (int i = n - 1; i > 0; --i) swap(a[i], a[rand() % (i + 1)]);
}
```
**2.排序核心：**

    插入排序：連續掃描並插入，適合小區間

    快速排序：三數取中 pivot + cutoff = 10，遞迴深度平均 O(log n)

    合併排序：自底向上 (iterative) 實作以減少遞迴成本

    堆排序：先建最大堆再反覆 extract‑max

    計時：std::chrono::high_resolution_clock 量測微秒，記錄總和與最大值

### 程式實作
***

### 效能分析
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
### 測試與驗證
***
**程式結果：**
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
#### 結論

### 申論及開發報告
***



