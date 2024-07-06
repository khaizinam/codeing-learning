#include <iostream>
#include "StackFrame.h"
#include "errors.h"
using namespace std;

/*
Run the testcase written in `filename`
@param filename name of the file
*/
void test(string filename) {
    StackFrame *sf = new StackFrame();
    try {
        sf->run(filename);
    }
    catch (exception& e) {
        cout << "exception : ";
        cout << e.what();
    }
    delete sf;
}

/*
Main function
*/
int main(int argc, char **argv) {
    test("testcase/test000.txt");
    return 0;
}