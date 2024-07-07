#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

#define INTEGER_TYPE 0
#define FLOAT_TYPE 1 
/*
Node
*/
class Node{
public:
    float val;
    int type; // 0 : int , 1: flat
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
public:
    LinkedList();

    void push(Node* newNode); // push bash, increse size by 1;

    Node* pop(); // create new Node from tail -> return that node -> delete tail;

    Node* top(); // create new Node from tail -> return that node;

    Node* getIndex(int index); // get Node from index start from head;

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

    void iconst(float val);

    void fconst(float val);

    void fconst(float val);
    
    void top();
};

#endif // !__STACK_FRAME_H__