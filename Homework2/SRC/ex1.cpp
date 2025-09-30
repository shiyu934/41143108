#include <iostream>
#include <cmath>
using namespace std;


class Polynomial;

// Term 
class Term {
    friend class Polynomial;
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef;   // 系數
    int exp;      // 指數
};

// Polynomial
class Polynomial {
public:
    Polynomial();                           
    ~Polynomial();                          
    Polynomial(const Polynomial&);         
    Polynomial& operator=(const Polynomial&); 

    Polynomial Add(const Polynomial& poly);   //加法
    Polynomial Mult(const Polynomial& poly);  //乘法
    float Eval(float f);                      //求值

    
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;  // 
    int capacity;     // 
    int terms;        //

    void Resize(int newCapacity); // 
};

Polynomial::Polynomial() {
    capacity = 10;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

Polynomial::Polynomial(const Polynomial& other) {
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; ++i)
        termArray[i] = other.termArray[i];
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        delete[] termArray;
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i)
            termArray[i] = other.termArray[i];
    }
    return *this;
}

void Polynomial::Resize(int newCapacity) {
    Term* newArray = new Term[newCapacity];
    for (int i = 0; i < terms; ++i)
        newArray[i] = termArray[i];
    delete[] termArray;
    termArray = newArray;
    capacity = newCapacity;
}

istream& operator>>(istream& in, Polynomial& poly) {
    cout << "Enter number of terms: ";
    in >> poly.terms;

    if (poly.terms > poly.capacity)
        poly.Resize(poly.terms);

    cout << "Enter each term as: coefficient exponent\n";
    for (int i = 0; i < poly.terms; ++i) {
        cout << "Term " << i + 1 << ": ";
        in >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }

    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; ++i) {
        out << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        if (i != poly.terms - 1)
            out << " + ";
    }
    return out;
}

float Polynomial::Eval(float f) {
    float result = 0.0;
    for (int i = 0; i < terms; ++i) {
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

Polynomial Polynomial::Add(const Polynomial& poly) {
    Polynomial result;
    int i = 0, j = 0;

    while (i < this->terms && j < poly.terms) {
        if (termArray[i].exp == poly.termArray[j].exp) {
            float sumCoef = termArray[i].coef + poly.termArray[j].coef;
            if (sumCoef != 0) {
                result.termArray[result.terms].coef = sumCoef;
                result.termArray[result.terms].exp = termArray[i].exp;
                result.terms++;
            }
            i++; j++;
        } else if (termArray[i].exp > poly.termArray[j].exp) {
            result.termArray[result.terms++] = termArray[i++];
        } else {
            result.termArray[result.terms++] = poly.termArray[j++];
        }
    }

    // ?理剩余?
    while (i < this->terms)
        result.termArray[result.terms++] = termArray[i++];
    while (j < poly.terms)
        result.termArray[result.terms++] = poly.termArray[j++];

    return result;
}

Polynomial Polynomial::Mult(const Polynomial& poly) {
    Polynomial result;

    for (int i = 0; i < this->terms; ++i) {
        for (int j = 0; j < poly.terms; ++j) {
            float newCoef = termArray[i].coef * poly.termArray[j].coef;
            int newExp = termArray[i].exp + poly.termArray[j].exp;

            // 合并同??
            bool found = false;
            for (int k = 0; k < result.terms; ++k) {
                if (result.termArray[k].exp == newExp) {
                    result.termArray[k].coef += newCoef;
                    found = true;
                    break;
                }
            }

            if (!found) {
                if (result.terms >= result.capacity)
                    result.Resize(result.capacity * 2);
                result.termArray[result.terms].coef = newCoef;
                result.termArray[result.terms].exp = newExp;
                result.terms++;
            }
        }
    }

    return result;
}

int main() {
    Polynomial p1, p2;

    cout << "Enter first polynomial:\n";
    cin >> p1;

    cout << "\nEnter second polynomial:\n";
    cin >> p2;

    cout << "\nFirst Polynomial: " << p1 << endl;
    cout << "Second Polynomial: " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    cout << "\nSum: " << sum << endl;

    Polynomial product = p1.Mult(p2);
    cout << "Product: " << product << endl;

    float x;
    cout << "\nEnter value to evaluate first polynomial: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}
