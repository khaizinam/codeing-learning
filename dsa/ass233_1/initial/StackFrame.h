#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

const int INTEGER_TYPE = 0;
const int  FLOAT_TYPE = 1;
/*
Node
*/
class Node{
public:
    float val;
    int type; // 0 : int , 1: flat
    int index;
    Node* next;

public:
    Node();

    Node(float val, int type);

    ~Node();
};

class LinkedList{
public:
    Node* head;
    Node* tail;
    int size;
    bool isStack;
    int maxSize;
public:
    LinkedList(bool isStack, int maxSize);

    void push(Node* newNode); // push bash, increse size by 1;

    Node* pop(); // create new Node from tail -> return that node -> delete tail;

    Node* top(); // create new Node from tail -> return that node;

    Node* getIndex(int index); // get Node from index start from head;
    
    void setData(Node* newNode, int index); // update new Node to index
    
    void print();

    ~LinkedList();
};

/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
    LinkedList* stack;
    LinkedList* memory;
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);

    void varConst(std::string code, float val);

    void istore(int index);

    void fstore(int index);

    void iload(int index);

    void fload(int index);

    void add(std::string code);

    void sub(std::string code);

    void mul(std::string code);

    void div(std::string code);

    void irem();

    void neg(std::string code);

    void iand();

    void ior();

    void ieq();

    void feq();

    void ineq();

    void fneq();

    void ilt();

    void flt();

    void igt();

    void fgt();

    void ibnot();

    void i2f();

    void f2i();

    void top();

    void val(int index);

    void print();
};

#endif // !__STACK_FRAME_H__