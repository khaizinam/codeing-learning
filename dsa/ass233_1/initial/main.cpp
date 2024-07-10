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
    int max = 4;
    int flag = 0;
    char filename[50]; // Assuming a reasonable maximum length for filename
    char output_file_name[50]; // Assuming a reasonable maximum length for filename
    char sol_file_name[50]; // Assuming a reasonable maximum length for filename
    
    while (flag <= max) {
        sprintf(filename, "testcase/test%d.txt", flag);

        sprintf(output_file_name, "output/out%d.txt", flag);

        sprintf(sol_file_name, "solution/sol%d.txt", flag);

        freopen(output_file_name,"w",stdout);

        test(filename);

        flag++;
    }

    return 0;
}