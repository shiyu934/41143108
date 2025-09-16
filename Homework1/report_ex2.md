# 41143108

作業一

## 解題說明
如果 S 是一個有 n 個元素的集合，powerset（冪集）是 S 的所有子集的集合。

例如，若
S = (a, b, c)，
則：

powerset(S) = { (), (a), (b), (c), (a,b), (a,c), (b,c), (a,b,c) }

### 解題策略

觀察子集的構成規則

對於每個元素（例如：a、b、c）：

可以出現在子集中；

也可以不出現在子集中。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>

using namespace std;

// 遞迴函式：產生冪集
void Powerset(char input[], char subset[], int n, int index, int Size) {
    // 終止條件：所有元素都已考慮完
    if (index == n) {
        cout << "{ ";
        for (int i = 0; i < Size; ++i) {
            cout << subset[i] << " ";
        }
        cout << "}" << endl;
        return;
    }

    // Case 1：不選擇 input[index]
    Powerset(input, subset, n, index + 1, Size);

    // Case 2：選擇 input[index]
    subset[Size] = input[index]; // 放進 subset 中
    Powerset(input, subset, n, index + 1, Size + 1);
}

int main() {
    char input[] = {'a', 'b', 'c'}; // 原始集合
    int n = sizeof(input) / sizeof(input[0]);

    char subset[10]; // 預設最大不超過 10 個元素（視情況可調整）

    cout << "集合 {a, b, c} 的冪集如下：" << endl;
    Powerset(input, subset, n, 0, 0);

    return 0;
}


```
## 效能分析

1. 時間複雜度：O(2^n)，因為每個元素有包含/不包含兩種選擇。
   
2.空間複雜度：O(n)（遞迴深度 + 子集長度）
## 測試與驗證

### 測試案例
```
{ }
{ c }
{ b }
{ b c }
{ a }
{ a c }
{ a b }
{ a b c }
```

### 編譯與執行指令

```shell
$ g++  powerset.cpp -o powerset
$ ./powerset
集合 {a, b, c} 的冪集如下：
{ }
{ c }
{ b }
{ b c }
{ a }
{ a c }
{ a b }
{ a b c }
```
### 結論

1. 冪集（Powerset）是指一個集合中所有子集的集合，包括：

   空集合 {}

   原集合本身

   所有可能的子集

   對一個長度為 n 的集合，冪集共有 2^n 個子集。

2.對於 set[index]：
    不選它 → 繼續往下處理下一個元素
    選它   → 把它加入 current，再處理下一個元素
    適合學習遞迴概念。


## 申論及開發報告
這類「列出所有組合可能性」的技巧廣泛應用在：

子集、組合、排列問題

背包問題（0/1 knapsack）
