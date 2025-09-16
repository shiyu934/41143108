#include <iostream>

using namespace std;

// 遞迴函式：產生冪集
void Powerset(char input[], char subset[], int n, int index, int Size) {
    // 終止條件：元素都考慮完
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

    char subset[10]; // 預設10 個元素

    cout << "集合 {a, b, c} 的冪集如下：" << endl;
    Powerset(input, subset, n, 0, 0);

    return 0;
}
