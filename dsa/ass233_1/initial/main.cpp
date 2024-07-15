#include <iostream>
#include "StackFrame.h"
#include "errors.h"
#include <fstream>
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
        cout << e.what();
    }
    delete sf;
}

/*
Main function
*/
int main(int argc, char **argv) {
    int max = 28;
    int flag = 10;
    char filename[50]; // Assuming a reasonable maximum length for filename
    char output_file_name[50]; // Assuming a reasonable maximum length for filename
    char sol_file_name[50]; // Assuming a reasonable maximum length for filename
    
    while (flag <= max) {
        sprintf(filename, "testcase/test0%d.txt", flag);

        sprintf(output_file_name, "output/out0%d.txt", flag);

        freopen(output_file_name,"w",stdout);

        test(filename);

        flag++;
    }
    return 0;
}