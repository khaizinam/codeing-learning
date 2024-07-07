/*Hãy hiện thực lớp Complex để thể hiện số phức, với các phương thức sau đây được cung cấp:

- Constructor với tối đa hai thông số tương ứng phần thực và phần ảo, khuyết một thông số thì thông số đó bằng 0.0

- getModulus để trả về giá trị modulo của số phức.

- Các toán tử >, >=, <, <=, ==, != để so sánh hai số phức dựa trên modulo. */

#include <iostream>
#include <cmath>
using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double real=0.0, double imag=0.0) {
        this->real = real;
        this->imag = imag;
    }

    double getModulus() const {
        return sqrt(real * real + imag * imag);
    }

    bool operator>(const Complex& other) const {
        return getModulus() > other.getModulus();
    }

    bool operator>=(const Complex& other) const {
        return getModulus() >= other.getModulus();
    }

    bool operator<(const Complex& other) const {
        return getModulus() < other.getModulus();
    }

    bool operator<=(const Complex& other) const {
        return getModulus() <= other.getModulus();
    }

    bool operator==(const Complex& other) const {
        return getModulus() == other.getModulus();
    }

    bool operator!=(const Complex& other) const {
        return getModulus() != other.getModulus();
    }
};

// Example usage:
int main() {
    Complex c1(3, 4);  // 3 + 4i
    Complex c2(1, -1); // 1 - i
    
    cout << c1.getModulus() << endl;  // Output: 5
    cout << c2.getModulus() << endl;  // Output: 1.41421
    
    return 0;
}