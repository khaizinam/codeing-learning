/* Hãy hiện thực các phương thức sort của lớp StraightSelectionSort để các phương thức này có thể sắp xếp tăng dần với dữ liệu là mảng các phần tử có kiểu Complex:

- Phương thức static void sort(Complex *list, int length): sắp xếp toàn bộ phẩn trong list.

- Phương thức static void sort(Complex *list, int length, int left, int right): sắp xếp các phần tử có chỉ số nằm trong đoạn [left, right]. (không cần kiểm tra tính đúng đắn của các giá trị truyền vào).

Sinh viên phải sử dụng mã giả đã được viết trong slide để viết thành mã thật.

For example:

Test	Result
int length = 5;
Complex *list = new Complex[5];
list[0] = Complex(3, -1);
list[1] = Complex(5, 2);
list[2] = Complex(1, -7);
list[3] = Complex(6, 8);
list[4] = Complex(-2, 4);

StraightInsertionSort::sort(list, length);
3 - 1 * i: 3.16228
-2 + 4 * i: 4.47214
5 + 2 * i: 5.38516
1 - 7 * i: 7.07107
6 + 8 * i: 10
Number of comparision (on Complex): 10 */


#include <iostream>
#include <cmath> // For sqrt function
using namespace std;

// Define Complex class
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

    // Operators to compare modulus
    bool operator>(const Complex& other) const {
        return getModulus() > other.getModulus();
    }

    bool operator<(const Complex& other) const {
        return getModulus() < other.getModulus();
    }

    // Print method for testing
    void print() const {
        cout << "(" << real << " + " << imag << "i)" << endl;
    }
};


class StraightInsertionSort {
public:
    static void sort(Complex *list, int length) {
        sort(list, length, 0, length - 1);
    }

    static void sort(Complex *list, int length, int left, int right) {
        if (length > 1) {
            int curr = left + 1;
            while (curr <= right) {
                Complex tmp = list[curr];
                int step = curr - 1;
                while (step >= left && tmp < list[step]) {
                    list[step + 1] = list[step];
                    step--;
                }
                list[step + 1] = tmp;
                curr++;
            }
        }
    }
};

// Test the StraightSelectionSort with Complex numbers
int main() {
    Complex arr[] = { Complex(3, 4), Complex(1, -1), Complex(2, 2), Complex(0, 1) };
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array:" << endl;
    for (int i = 0; i < n; ++i) {
        arr[i].print();
    }

    // Sort entire array
    StraightInsertionSort::sort(arr, n);

    cout << "\nArray after sorting:" << endl;
    for (int i = 0; i < n; ++i) {
        arr[i].print();
    }

    // Sort within range [1, 3]
    cout << "\nSorting elements from index 1 to 3:" << endl;
    StraightInsertionSort::sort(arr, n, 1, 3);

    cout << "\nArray after partial sorting:" << endl;
    for (int i = 0; i < n; ++i) {
        arr[i].print();
    }

    return 0;
}
