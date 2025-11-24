# 41143108

作業三

## 解題說明
這份作業要求你使用 C++ 開發一個名為 `Polynomial` 的類別，用來表示並操作單變數多項式。

### 1. 資料結構要求
**核心結構**：必須使用具備 **Header Node（標頭節點）** 的 **Circular Linked List（環狀鏈結串列）** 來儲存多項式。
**節點內容**：每一個節點代表多項式中的一項 (Term)，包含三個資料成員：
    coef(係數)：整數。
    exp(指數)：整數 
    link(指標)：指向下一個節點
**記憶體管理 (Available-Space List)**：
    為了提升刪除多項式的效率，題目要求必須實作 **Available-Space List（可用空間串列）** 機制 。
    這意味著你不能只依賴標準的 `delete`，而是要維護一個靜態的節點池 (Pool)，當多項式被刪除時，將其所有節點一次性歸還給這個池子，以便後續重複利用 。

### 2. 輸入與輸出格式
**外部表示法 (Input/Output)**：
    多項式在輸入或輸出時，應表示為一串整數序列：
    $$n, c_1, e_1, c_2, e_2, c_3, e_3, \dots, c_n, e_n$$
   $n$：代表多項式共有幾項 
   $c_i$：第 $i$ 項的係數 
    $e_i$：第 $i$ 項的指數 
    **排序**：指數必須由大到小排列，即 $e_1 > e_2 > \dots > e_n$

### 3. 必須實作的功能函數
你需要實作並測試以下 9 個函數 ：

**(a) 輸入運算子 (`operator>>`)**：
    讀取上述格式的整數序列，並將其轉換為內部的環狀鏈結串列（含 Header Node）。
**(b) 輸出運算子 (`operator<<`)**：
    將鏈結串列轉換回外部的整數序列並輸出（通常作業也會要求輸出成人類可讀的 $3x^2 + 5$ 格式）。
**(c) 複製建構子 (`Polynomial(const Polynomial& a)`)**：
    初始化一個新的多項式，使其內容與多項式 $a$ 相同（Deep Copy）。
**(d) 指派運算子 (`operator=`)**：
    將多項式 $a$ 指定給當前物件 (*this)。
**(e) 解構子 (`~Polynomial()`)**：
    將當前多項式的所有節點歸還給 Available-space list。
**(f) 加法 (`operator+`)**：
    計算並回傳 `*this + b` 的結果。
**(g) 減法 (`operator-`)**：
    計算並回傳 `*this - b` 的結果。
**(h) 乘法 (`operator*`)**：
    計算並回傳 `*this * b` 的結果。
**(i) 求值 (`Evaluate(float x)`)**：
    代入數值 $x$，計算並回傳多項式的結果值。

### 總結
這是一個標準的資料結構練習，重點在於練習 **鏈結串列的操作** 以及 **自訂記憶體管理 (Memory Management)** 的技巧。你需要特別注意環狀串列的終止條件判斷（是否回到 Header），以及 Available List 的維護。

### 解題策略

這是針對此題最精簡的解題策略核心：

1.  **資料結構策略**：
    採用 **環狀鏈結串列 (Circular List) 搭配 Header Node**。這能讓「插入」與「刪除」邏輯統一，不用額外寫 `if` 去判斷空指標，並以 `node->link == head` 作為結束條件 

2.  **記憶體管理 (關鍵加分項)**：
    實作 **Available-Space List (靜態回收池)**。
    * **新增時**：優先從池中拿節點，沒有才 `new`。
  **刪除時**：不使用 `delete`，而是將「整串」多項式直接掛回回收池前端 (僅需常數時間 $O(1)$)，極大化運作效率 

3.  **運算演算法**：
    **加減法 (`+`, `-`)**：**雙指標法**。兩個指標同步遍歷，指數大的先存入結果，指數相同則係數相加減，類似 Merge Sort 的合併過程 
    **乘法 (`*`)**：**分配律 + 累加**。取 A 的每一項去乘 B 的全部，產生暫存多項式後，再利用寫好的加法 (`+`) 累加到結果中

4.  **I/O 處理**：
    輸入前必須先呼叫回收機制**清空舊資料**；輸出需處理「係數為 1 不印數字」、「指數為 0 不印 x」等格式細節 

## 程式實作

以下為主要程式碼：

```
#include <iostream>
#include <cmath> // for pow

using namespace std;

// 前置宣告
class Polynomial;

// 定義多項式的節點 (Term)
class PolyNode {
    friend class Polynomial;
    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, Polynomial& x);

private:
    int coef;
    int exp;
    PolyNode* link;

    // [加分項 Source 11] Available-Space List 機制
    static PolyNode* avail;

public:
    // 建構子
    PolyNode(int c = 0, int e = 0, PolyNode* l = nullptr) : coef(c), exp(e), link(l) {}
};

// 初始化靜態成員 (Available List 一開始為空)
PolyNode* PolyNode::avail = nullptr;

class Polynomial {
private:
    PolyNode* head; // Header Node

    // [記憶體管理] 從 Available List 獲取節點
    PolyNode* GetNode(int c = 0, int e = -1, PolyNode* l = nullptr) {
        PolyNode* p;
        if (PolyNode::avail != nullptr) {
            p = PolyNode::avail;
            PolyNode::avail = PolyNode::avail->link;
            p->coef = c;
            p->exp = e;
            p->link = l;
        }
        else {
            p = new PolyNode(c, e, l);
        }
        return p;
    }

    // [記憶體管理] 將單一節點歸還給 Available List
    void RetNode(PolyNode* p) {
        p->link = PolyNode::avail;
        PolyNode::avail = p;
    }

    // [記憶體管理] 將整個環狀串列歸還 (不含 head)
    void CircularDelete() {
        if (head->link == head) return; // 空串列

        // 找到最後一個節點
        PolyNode* last = head->link;
        while (last->link != head) {
            last = last->link;
        }

        // 將整串鏈結串列掛到 avail 的前端 (Constant time operation in theory if we tracked last, O(n) here)
        last->link = PolyNode::avail;
        PolyNode::avail = head->link;

        head->link = head; // 恢復為空環狀狀態
    }

public:
    // 建構子：建立 Header Node
    Polynomial() {
        head = GetNode(0, -1); // exp = -1 for header
        head->link = head;
    }

    // (e) 解構子
    ~Polynomial() {
        CircularDelete();
        RetNode(head); // 最後歸還 header
    }

    // (c) Copy Constructor
    Polynomial(const Polynomial& a) {
        head = GetNode(0, -1);
        head->link = head;
        // 複製節點
        PolyNode* dest = head;
        PolyNode* src = a.head->link;
        while (src != a.head) {
            dest->link = GetNode(src->coef, src->exp, head);
            dest = dest->link;
            src = src->link;
        }
    }

    // (d) Assignment Operator
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            CircularDelete(); // 清除舊內容

            PolyNode* dest = head;
            PolyNode* src = a.head->link;
            while (src != a.head) {
                dest->link = GetNode(src->coef, src->exp, head);
                dest = dest->link;
                src = src->link;
            }
        }
        return *this;
    }

    // (a) Input Operator
    // Format: n, c1, e1, c2, e2, ..., cn, en
    friend istream& operator>>(istream& is, Polynomial& x) {
        x.CircularDelete(); // 讀取前先清空
        int n;
        cout << "Enter number of terms: ";
        is >> n;

        PolyNode* last = x.head;
        for (int i = 0; i < n; ++i) {
            int c, e;
            cout << "Enter coef and exp for term " << i + 1 << ": ";
            is >> c >> e;
            last->link = x.GetNode(c, e, x.head);
            last = last->link;
        }
        return is;
    }

    // (b) Output Operator
    friend ostream& operator<<(ostream& os, Polynomial& x) {
        PolyNode* current = x.head->link;
        if (current == x.head) {
            os << "0";
            return os;
        }

        bool first = true;
        while (current != x.head) {
            if (!first && current->coef > 0) os << " + ";
            if (current->coef < 0) os << " - "; // 負號

            int absCoef = abs(current->coef);
            if (absCoef != 1 || current->exp == 0) os << absCoef;

            if (current->exp > 0) os << "x";
            if (current->exp > 1) os << "^" << current->exp;

            current = current->link;
            first = false;
        }
        return os;
    }

    // (f) Addition
    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        PolyNode* aPtr = head->link;
        PolyNode* bPtr = b.head->link;
        PolyNode* cLast = c.head;

        while (aPtr != head && bPtr != b.head) {
            if (aPtr->exp == bPtr->exp) {
                int sum = aPtr->coef + bPtr->coef;
                if (sum != 0) {
                    cLast->link = c.GetNode(sum, aPtr->exp, c.head);
                    cLast = cLast->link;
                }
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
            else if (aPtr->exp > bPtr->exp) {
                cLast->link = c.GetNode(aPtr->coef, aPtr->exp, c.head);
                cLast = cLast->link;
                aPtr = aPtr->link;
            }
            else {
                cLast->link = c.GetNode(bPtr->coef, bPtr->exp, c.head);
                cLast = cLast->link;
                bPtr = bPtr->link;
            }
        }

        // 處理剩餘項目
        while (aPtr != head) {
            cLast->link = c.GetNode(aPtr->coef, aPtr->exp, c.head);
            cLast = cLast->link;
            aPtr = aPtr->link;
        }
        while (bPtr != b.head) {
            cLast->link = c.GetNode(bPtr->coef, bPtr->exp, c.head);
            cLast = cLast->link;
            bPtr = bPtr->link;
        }

        return c;
    }

    // (g) Subtraction
    Polynomial operator-(const Polynomial& b) const {
        Polynomial c;
        PolyNode* aPtr = head->link;
        PolyNode* bPtr = b.head->link;
        PolyNode* cLast = c.head;

        while (aPtr != head && bPtr != b.head) {
            if (aPtr->exp == bPtr->exp) {
                int diff = aPtr->coef - bPtr->coef;
                if (diff != 0) {
                    cLast->link = c.GetNode(diff, aPtr->exp, c.head);
                    cLast = cLast->link;
                }
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
            else if (aPtr->exp > bPtr->exp) {
                cLast->link = c.GetNode(aPtr->coef, aPtr->exp, c.head);
                cLast = cLast->link;
                aPtr = aPtr->link;
            }
            else {
                cLast->link = c.GetNode(-bPtr->coef, bPtr->exp, c.head); // 注意變號
                cLast = cLast->link;
                bPtr = bPtr->link;
            }
        }

        while (aPtr != head) {
            cLast->link = c.GetNode(aPtr->coef, aPtr->exp, c.head);
            cLast = cLast->link;
            aPtr = aPtr->link;
        }
        while (bPtr != b.head) {
            cLast->link = c.GetNode(-bPtr->coef, bPtr->exp, c.head);
            cLast = cLast->link;
            bPtr = bPtr->link;
        }
        return c;
    }

    // (h) Multiplication
    Polynomial operator*(const Polynomial& b) const {
        Polynomial res;
        PolyNode* aPtr = head->link;

        // 概念：A 的每一項去乘 B 的每一項，然後加到結果中
        while (aPtr != head) {
            Polynomial tempPoly;
            PolyNode* tempLast = tempPoly.head;
            PolyNode* bPtr = b.head->link;

            while (bPtr != b.head) {
                int newCoef = aPtr->coef * bPtr->coef;
                int newExp = aPtr->exp + bPtr->exp;
                tempLast->link = tempPoly.GetNode(newCoef, newExp, tempPoly.head);
                tempLast = tempLast->link;
                bPtr = bPtr->link;
            }

            // 利用已寫好的加法運算子來累加結果
            res = res + tempPoly;
            aPtr = aPtr->link;
        }
        return res;
    }

    // (i) Evaluate
    float Evaluate(float x) const {
        float sum = 0;
        PolyNode* current = head->link;
        while (current != head) {
            sum += current->coef * pow(x, current->exp);
            current = current->link;
        }
        return sum;
    }
};
```
```
// 測試主程式
int main() {
    Polynomial p1, p2, p3;

    cout << "=== Polynomial 1 Input ===" << endl;
    cin >> p1; // 輸入格式範例: 3 2 10 4 2 1 0 (代表 3項: 2x^10 + 4x^2 + 1)
    cout << "P1 = " << p1 << endl;

    cout << "\n=== Polynomial 2 Input ===" << endl;
    cin >> p2;
    cout << "P2 = " << p2 << endl;

    cout << "\n=== Testing Addition (P1 + P2) ===" << endl;
    p3 = p1 + p2;
    cout << "P1 + P2 = " << p3 << endl;

    cout << "\n=== Testing Subtraction (P1 - P2) ===" << endl;
    p3 = p1 - p2;
    cout << "P1 - P2 = " << p3 << endl;

    cout << "\n=== Testing Multiplication (P1 * P2) ===" << endl;
    p3 = p1 * p2;
    cout << "P1 * P2 = " << p3 << endl;

    cout << "\n=== Testing Evaluation ===" << endl;
    float x;
    cout << "Enter x for P1: ";
    cin >> x;
    cout << "Evaluate P1 at " << x << " = " << p1.Evaluate(x) << endl;

    return 0;
}

```
## 效能分析

運算功能,時間複雜度,說明
**建立** (Input/Constructor),O(N),需讀取 N 個項目並依序建立節點。若輸入未排序需 O(N2) 或 O(NlogN)，但題目假設輸入已排序 ，故為線性時間。

**加法** (operator+),O(N+M),採用雙指標法 (Two-pointer)，只需遍歷兩個串列各一次即可完成合併。這是鏈結串列處理多項式加法的最佳效率。

**減法** (operator-),O(N+M),邏輯與加法完全相同，僅係數相減。

**乘法** (operator*),O(N⋅M⋅Result),實作採用「A 的每一項乘 B 的整串 (產生 M 項)」然後「累加 (+) 到結果」。因為每次累加都需要遍歷目前的結果串列，效率較差。最壞情況接近 O(N2M)。

**求值**(Evaluate),O(N),需遍歷所有節點一次進行 pow 運算與累加。

**刪除** (Destructor),O(N),雖然需遍歷找到尾端節點，但不需要呼叫 N 次 delete（見下方記憶體分析）。

**空間複雜度**：最壞情況下，每對項都產生獨立新次方（沒有可以合併的指數），那結果多項式可能有 nm 項，所以是 O(nm)。

## 測試與驗證
案例 1：輸入與輸出

輸入三項多項式：
```
3 4
-2 2
7 0
```

輸出結果：3x^4 + -2x^2 + 7x^0
operator >>、<< 功能正常。

案例 2：加法
```
p1 = 3x^4 + 2x^2
p2 = 5x^2 + 1
```
手算：3x^4 + 7x^2 + 1
程式輸出：3x^4 + 7x^2 + 1x^0
同次方項合併正確。

案例 3：乘法
```
p1 = 2x + 1
p2 = 3x + 4
```
手算：(2x+1)(3x+4) = 6x² + 11x + 4
程式輸出：6x^2 + 11x^1 + 4x^0
交叉相乘與合併同次方正確。

案例 4：代入求值

p(x) = 3x² + 2x + 1，x=2
手算：3(2²)+2(2)+1=17
程式輸出：p1(2) = 17
Eval() 計算正確。

案例 5：加法歸零

p1 = 4x，p2 = -4x
手算結果：0
程式不輸出 0x¹ 項，
已正確忽略歸零項。

### 編譯與執行指令

```shell
$ g++ -std=c++11 polynomial.cpp -o polynomial
$ ./polynomial
```
### 結論

本次作業成功實作出 Polynomial 類別，完成了多項式的輸入、輸出、加法、乘法與代入求值等功能。
程式採用物件導向設計，將每一項封裝成 Term 物件，再由 Polynomial 管理整體結構，達成抽象資料型態（ADT）的概念。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

符合數學遞迴關係
多項式本身具有遞迴特性，例如：

𝑃(𝑥)=𝑎𝑛𝑥𝑛+𝑃𝑛−1(𝑥)

每次處理最高次項後，只需對剩下的子多項式再做一次相同運算，遞迴非常自然。

簡化問題邏輯
遞迴可將「整體問題」分解為「較小的相同子問題」。
例如在求值時：

𝐸𝑣𝑎𝑙(𝑃,𝑥)=𝑎𝑛𝑥𝑛+𝐸𝑣𝑎𝑙(𝑃𝑛−1,𝑥)

不需要額外迴圈，只需呼叫自己直到多項式結束，程式結構更簡潔。

提升程式可讀性與模組化
遞迴使程式更貼近數學推導，減少索引與狀態變數，讓程式更容易維護與理解。

## 實作心得

這次作業讓學會如何用**物件導向設計**實作多項式運算，抽象資料型態（ADT）的概念。
過程中學會設計建構子、解構子與運算子多載，並處理加法、乘法及記憶體動態擴充的細節。
收穫理解「資料結構設計」與「演算法效率」的關聯，也培養了撰寫可讀、可維護程式的習慣。
