#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

// Default constructor
Node::Node() : val(0), type(0), next(nullptr) {}

// Constructor with parameters
Node::Node(float value, int nodeType) : val(value), type(nodeType), next(nullptr) {}

// Destructor
Node::~Node() {
    // No dynamic memory cleanup needed in this example
}

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

void LinkedList::push(Node* newNode) {
    if (head == nullptr) {
        this->head = newNode;
        this->tail = newNode;
    } else {
        this->tail->next = newNode;
        this->tail = newNode;
    }

    cout << "insert Node(" << newNode->val << "," << newNode->type << ");\n";

    size++;
}

Node* LinkedList::pop() {
    if (this->head == nullptr) {
        return nullptr;
    }

    Node* current = this->head;
    Node* previous = nullptr;

    while (current->next != nullptr) {
        previous = current;
        current = current->next;
    }

    if (previous != nullptr) {
        previous->next = nullptr;
        this->tail = previous;
    } else {
        this->head = nullptr;
        this->tail = nullptr;
    }

    this->size--;
    return current;
}

Node* LinkedList::top() {
    return this->tail;
}

Node* LinkedList::getIndex(int index) {
    if (index < 0 || index >= this->size) {
        return nullptr;
    }

    Node* current = this->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

LinkedList::~LinkedList() {
    // Destructor: Clean up all nodes in the list
    Node* current = head;
    Node* next = nullptr;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
}


StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE), stack(new LinkedList()), memory(new LinkedList()) {}

void StackFrame::iconst(float val) { 
    Node* newNode = new Node(val, 0);
    this->stack->push(newNode);
}

void StackFrame::fconst(float val) { 
    Node* newNode = new Node(val, 1);
    this->stack->push(newNode);
}

void StackFrame::top(){
    Node* tmp = this->stack->top();
    if(tmp->type == 1)
        cout << (int)tmp->val << "\n";
    else 
        cout << tmp->val << "\n";
}

void StackFrame::run(string filename) {

    std::fstream file(filename, std::ios::in); // Open the file for reading

    if (!file.is_open()) { return; }

    std::string javmCode;
    float value = 0.0f;

    while (file >> javmCode) { // Read each "word" (token) from the file
        // Create Node objects based on javmCode and value
        if (javmCode == "iconst") {
            file >> value; 
            this->iconst(value);

        } else if(javmCode == "fconst"){
            file >> value; 
            this->fconst(value);

        } else if(javmCode == "top") {
            this->top();
        }

        // Add more conditions for other javmCode types if needed
    }

    file.close(); // Close the file
}