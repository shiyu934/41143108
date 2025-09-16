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

| 測試案例 | 輸入參數 $m, n$ | 預期結果 | 實際結果 |
| ---- | ----------- | ---- | ---- |
| 測試一  | 0, 0        | 1    | 1    |
| 測試二  | 1, 0        | 2    | 2    |
| 測試三  | 0, 3        | 4    | 4    |
| 測試四  | 2, 1        | 5    | 5    |
| 測試五  | -1, 2       | 拋出例外 | 拋出例外 |
| 測試六  | 2, -3       | 拋出例外 | 拋出例外 |

### 編譯與執行指令

```shell
$ g++ ackermann.cpp -o ackermann
$ ./ackermann
3 2
29
```
### 結論

1. 程式成功遞迴實作 Ackermann 函數，對小範圍參數能正確計算。

2.編譯與執行簡單，但對較大參數可能爆棧或數值溢位。

3.可透過迭代或記憶化改進效能，適合學習遞迴概念。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   Ackermann 函數本身就是遞迴定義，遞迴程式碼能直接反映數學公式
   當 $m=0$ 或 $n=0$ 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   程式結構簡潔清晰，使用遞迴可省略手動模擬堆疊或迴圈，使程式碼簡單且易於維護，例如：
```
unsigned long long Ackermann(unsigned int m, unsigned int n) {
    if (m == 0) return n + 1;
    else if (n == 0) return Ackermann(m - 1, 1);
    else return Ackermann(m - 1, Ackermann(m, n - 1));
}
```
3. **遞迴的語意清楚**  
在程式當中，每一次遞迴呼叫都對應一個「子問題」，最終結果會隨著遞迴逐層返回組合，完成計算。設計簡化程式邏輯，不需額外temp來儲存。

透過遞迴實作 Ackermann 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $m $n 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
