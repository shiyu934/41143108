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

    // [加分項 Source 11] 
    static PolyNode* avail;

public:
   
    PolyNode(int c = 0, int e = 0, PolyNode* l = nullptr) : coef(c), exp(e), link(l) {}
};

// 初始化
PolyNode* PolyNode::avail = nullptr;

class Polynomial {
private:
    PolyNode* head; // Header Node

    // 記憶體管理
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


    void RetNode(PolyNode* p) {
        p->link = PolyNode::avail;
        PolyNode::avail = p;
    }

 
    void CircularDelete() {
        if (head->link == head) return; // 空串列

        // 找到最後一個節點
        PolyNode* last = head->link;
        while (last->link != head) {
            last = last->link;
        }

      
        last->link = PolyNode::avail;
        PolyNode::avail = head->link;

        head->link = head; // 恢復為空環狀狀態
    }

public:
    // 建構子：
    Polynomial() {
        head = GetNode(0, -1); // exp = -1 for header
        head->link = head;
    }

    // (e) 解構子
    ~Polynomial() {
        CircularDelete();
        RetNode(head); 
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
