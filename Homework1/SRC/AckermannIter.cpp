#include <iostream>
#include <stack>
using namespace std;

struct Frame {
    unsigned int m, n;
    bool pending;  // 用來記錄是否已經展開過 A(m, n-1)
};

unsigned long long AckermannIter(unsigned int m, unsigned int n) {
    stack<Frame> st;
    st.push({m, n, false});
    unsigned long long result = 0;

    while (!st.empty()) {
        Frame &f = st.top();
        if (f.m == 0) {
            result = f.n + 1;
            st.pop();
        } else if (f.n == 0) {
            st.pop();
            st.push({f.m - 1, 1, false});
        } else if (!f.pending) {
            f.pending = true;
            st.push({f.m, f.n - 1, false});
        } else {
            st.pop();
            st.push({f.m - 1, (unsigned int)result, false});
        }
    }
    return result;
}

int main() {
    unsigned int m, n;
    cout << "請輸入 m 與 n: ";
    cin >> m >> n;

    cout << "Ackermann(" << m << "," << n << ") = "
         << AckermannIter(m, n) << endl;

    return 0;
}
