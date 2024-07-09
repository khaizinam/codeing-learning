#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;
int _LINE_ = 1;
// Default constructor
Node::Node() : val(0), type(0), next(nullptr) {}

// Constructor with parameters
Node::Node(float value, int nodeType) : val(value), type(nodeType), next(nullptr) {}

// Destructor
Node::~Node() {
    // No dynamic memory cleanup needed in this example
}

LinkedList::LinkedList(bool isStack, int maxSize) : head(nullptr), tail(nullptr), size(0),isStack(isStack),maxSize(maxSize)  {}

void LinkedList::push(Node* newNode) {

    if(this->size >= this->maxSize ){
        if(this->isStack)
            throw StackFull(_LINE_);
        else 
            throw ArrayOutOfRange(_LINE_);
    };

    if (this->head == nullptr) {
        this->head = newNode;
        this->tail = newNode;
    } else {
        this->tail->next = newNode;
        this->tail = newNode;
    }
    this->size += 1;
}

Node* LinkedList::pop() {

    if (this->head == nullptr) {
        return nullptr;
    }

    if(this->size == 1){
        Node* tmp = this->head;
        this->head = nullptr;
        this->tail = nullptr;
        this->size--;
        return tmp;
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

void LinkedList::setData(Node* newNode, int index){
    if (index < 0) {
        return;
    }

    Node* tmp = this->head;

    int walker = 0;
    while (tmp != nullptr && walker < index) {
        tmp = tmp->next;
        walker++;
    }

    if (tmp == nullptr) {
        return;
    }

    tmp->val = newNode->val;
    tmp->type = newNode->type;
}

void LinkedList::print(){
    if(this->size > 0){
        Node* current = this->head;
        cout << current->val << ", " << current->type;
        while (current->next != nullptr)
        {
            current = current->next;
            cout << ", " << current->val << ", " << current->type;
        }
    }
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

void StackFrame::store(string code, int index) {
    if( this->stack->size == 0 )
        throw StackEmpty(_LINE_);

    if( (code == "istore" && this->stack->tail->type != INTEGER_TYPE )
    || (code == "fstore" && this->stack->tail->type != FLOAT_TYPE ))
        throw TypeMisMatch(_LINE_);

    int reIndex = index / 2;

    // insert new adress
    if(reIndex >= this->memory->size || reIndex == 0){
        Node* tmp = this->stack->pop();
        this->memory->push(tmp);

    }else{
        Node* tmp = this->stack->pop();
        this->memory->setData(tmp, reIndex);
    }
}

void StackFrame::varLoad(string code, int index) {
    if( this->memory->size == 0 ) 
        throw UndefinedVariable(_LINE_);
    
    int reIndex = index / 2;
   
    // insert new adress
    if(reIndex <= this->memory->size){
        Node* tmp = this->memory->getIndex(reIndex);
        
        if(tmp == nullptr ) 
            throw UndefinedVariable(_LINE_);
        
        if(code == "iload" && tmp->type != INTEGER_TYPE) 
            throw TypeMisMatch(_LINE_);

        if(code == "fload" && tmp->type != FLOAT_TYPE) 
            throw TypeMisMatch(_LINE_);

        Node* newNode = new Node(tmp->val, tmp->type);
        this->stack->push(newNode);
    }
}

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE), stack(new LinkedList(true, this->opStackMaxSize)), memory(new LinkedList(false, this->localVarArrSize)) {}

void StackFrame::print() {
    cout << "stack: <";
    this->stack->print();
    cout << ">\n";

    cout << "memory: [";
    this->memory->print();
    cout << "]\n";
    cout << "------------------------------\n";
}

void StackFrame::varConst(string code, float val) { 
    int _type = 0; // integer default

    if(code == "fconst") _type = 1;

    Node* newNode = new Node(val, _type);
    this->stack->push(newNode);
}

void StackFrame::add(string code){
    
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    bool isInterType = code == "iadd";
    Node* tmp = this->stack->pop();
    
    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(isInterType){
        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val + (int)tmp->val;
    }
    else {
        if(tmp->type != FLOAT_TYPE)
            throw TypeMisMatch(_LINE_);
            
        this->stack->tail->val += tmp->val;
    }
}

void StackFrame::sub(string code){
    
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    bool isInterType = code == "isub";
    Node* tmp = this->stack->pop();
    
    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(isInterType){
        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val - (int)tmp->val;
    }
    else {
        if(tmp->type != FLOAT_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val -= tmp->val;
    }
}

void StackFrame::mul(string code){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    bool isInterType = code == "imul";
    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(isInterType){
        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val * (int)tmp->val;
    }
    else {
        if(tmp->type != FLOAT_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val *= tmp->val;
    }
}

void StackFrame::div(string code){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);
    
    bool isInterType = code == "idiv";
    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->val == 0 )
        throw DivideByZero(_LINE_);

    if(isInterType){
        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val / (int)tmp->val;
    }
    else {
        if(tmp->type != FLOAT_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val /= tmp->val;
    }
}

void StackFrame::irem(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    if(tmp->val == 0 )
        throw DivideByZero(_LINE_);
        
    this->stack->tail->val = (int)this->stack->tail->val % (int)tmp->val;
}

void StackFrame::top(){
    Node* tmp = this->stack->top();
    if(tmp == nullptr) return;

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
        cout << javmCode << "\n";

        if (javmCode == "iconst" || javmCode == "fconst") {
            
            file >> value; 
            this->varConst(javmCode, value);

        } else if( javmCode == "istore" || javmCode == "fstore"){
            
            file >> value; 
            this->store(javmCode, (int)value);

        }  else if(javmCode == "iload" || javmCode == "fload"){
            
            file >> value; 
            this->varLoad(javmCode, (int)value);

        } else if(javmCode == "iadd" || javmCode == "fadd"){
            
            this->add(javmCode);

        } else if(javmCode == "isub" || javmCode == "fsub"){
            
            this->sub(javmCode);

        } else if(javmCode == "imul" || javmCode == "fmul"){
            this->mul(javmCode);

        } else if(javmCode == "idiv" || javmCode == "fdiv"){

            this->div(javmCode);

        } else if(javmCode == "irem"){
            this->irem();
        }
        else if(javmCode == "top") {
            this->top();
        }

        this->print();
        _LINE_++;
    }

    file.close(); // Close the file
}