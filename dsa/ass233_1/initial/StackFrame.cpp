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
Node::Node(float value, int nodeType) : val(value), type(nodeType), index(-1), next(nullptr) {}

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

void LinkedList::pushMemory(Node* newNode, int index) {
    if (index < 0 || index >= this->maxSize - 1) {
        throw ArrayOutOfRange(_LINE_);
    }

    // If the list is empty
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    // If inserting at the head
    else if (index == 0) {
        newNode->next = head;
        head = newNode;
    }
    else {
        // Traverse to find the node just before the index position
        Node* current = head;
        int currentIndex = 0;
        while (current != nullptr && currentIndex < index - 1) {
            current = current->next;
            currentIndex++;
        }

        // Insert newNodeToInsert after current node
        newNode->next = current->next;
        current->next = newNode;

        // If inserted at the end, update tail
        if (newNode->next == nullptr) {
            tail = newNode;
        }
    }

    size++;
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

Node* LinkedList::findIndex(int index) {
    if (index < 0 || index >= this->maxSize - 1) {
        throw ArrayOutOfRange(_LINE_);
    }

    // Traverse the list to find the node with the specified index
    Node* current = head;
    while (current != nullptr) {
        if (current->index == index) {
            // Node with the specified index found
            return current;
        }
        current = current->next;
    }

    throw UndefinedVariable(_LINE_);
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

void StackFrame::istore(int index) {
    if( this->stack->size == 0 )
        throw StackEmpty(_LINE_);

    if( this->stack->tail->type != INTEGER_TYPE )
        throw TypeMisMatch(_LINE_);

    Node* tmp = this->stack->pop();
    tmp->index = index;
    this->memory->push(tmp);
}

void StackFrame::fstore(int index) {
    if( this->stack->size == 0 )
        throw StackEmpty(_LINE_);

    if( this->stack->tail->type != FLOAT_TYPE )
        throw TypeMisMatch(_LINE_);

    Node* tmp = this->stack->pop();
    tmp->index = index;
    this->memory->push(tmp);
}

void StackFrame::iload(int index) {
    if( this->memory->size == 0 ) 
        throw UndefinedVariable(_LINE_);
    
    Node* tmp = this->memory->findIndex(index);
    
    if(tmp == nullptr ) 
        throw UndefinedVariable(_LINE_);
    
    if(tmp->type != INTEGER_TYPE) 
        throw TypeMisMatch(_LINE_);

    Node* newNode = new Node(tmp->val, tmp->type);
    this->stack->push(newNode);
}

void StackFrame::fload(int index) {
    if( this->memory->size == 0 ) 
        throw UndefinedVariable(_LINE_);
    
    Node* tmp = this->memory->findIndex(index);
    
    if(tmp == nullptr ) 
        throw UndefinedVariable(_LINE_);
    
    if(tmp->type != FLOAT_TYPE) 
        throw TypeMisMatch(_LINE_);

    Node* newNode = new Node(tmp->val, tmp->type);
    this->stack->push(newNode);
}

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE), stack(new LinkedList(true, this->opStackMaxSize / 2)), memory(new LinkedList(false, this->localVarArrSize)) {}

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

    if(isInterType){
        if(tmp->type != this->stack->tail->type)
            throw TypeMisMatch(_LINE_);

        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val + (int)tmp->val;
    }
    else {       
        float a = this->stack->tail->val;
        float b = tmp->val;
        this->stack->tail->val = (float)(a + b);
        this->stack->tail->type = FLOAT_TYPE;
    }
}

void StackFrame::sub(string code){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    bool isInterType = code == "isub";
    Node* tmp = this->stack->pop();
    
    if(isInterType){
        if(tmp->type != this->stack->tail->type)
            throw TypeMisMatch(_LINE_);

        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val - (int)tmp->val;
    } else {       
        float a = this->stack->tail->val;
        float b = tmp->val;
        this->stack->tail->val = (float)(a - b);
        this->stack->tail->type = FLOAT_TYPE;
    }
}

void StackFrame::mul(string code){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    bool isInterType = code == "imul";
    Node* tmp = this->stack->pop();

    if(isInterType){
        if(tmp->type != this->stack->tail->type)
            throw TypeMisMatch(_LINE_);

        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        this->stack->tail->val = (int)this->stack->tail->val * (int)tmp->val;
    } else {       
        float a = this->stack->tail->val;
        float b = tmp->val;
        this->stack->tail->val = (float)(a * b);
        this->stack->tail->type = FLOAT_TYPE;
    }
}

void StackFrame::div(string code){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);
    
    bool isInterType = code == "idiv";
    Node* tmp = this->stack->pop();

    if(tmp->val == 0 )
        throw DivideByZero(_LINE_);

    if(isInterType){
        if(tmp->type != this->stack->tail->type)
            throw TypeMisMatch(_LINE_);

        if(tmp->type != INTEGER_TYPE)
            throw TypeMisMatch(_LINE_);

        int a = this->stack->tail->val;
        int b = tmp->val;
        this->stack->tail->val = (int)(a / b);
    } else {       
        float a = this->stack->tail->val;
        float b = tmp->val;
        this->stack->tail->val = (float)(a / b);
        this->stack->tail->type = FLOAT_TYPE;
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

void StackFrame::neg(string code){

    bool isIntegerType = code == "ineg";

    if(this->stack->size == 0)
        throw StackEmpty(_LINE_);

    if ( isIntegerType && this->stack->tail->type != INTEGER_TYPE ){
        throw TypeMisMatch(_LINE_);
    }

    if(isIntegerType){  
        this->stack->tail->val = (int)(this->stack->tail->val * -1);
    } else {
        this->stack->tail->val = this->stack->tail->val * -1;
        this->stack->tail->type = FLOAT_TYPE;
    }
}

void StackFrame::iand(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);
    
    this->stack->tail->val= (int)((int)tmp->val & (int)this->stack->tail->val );
}

void StackFrame::ior(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);
    
    this->stack->tail->val= (int)((int)tmp->val | (int)this->stack->tail->val );
}

void StackFrame::ieq(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    int a = (int)this->stack->tail->val;
    int b = (int)tmp->val;

    bool result = a == b;

    this->stack->tail->val = result ? 1 : 0;
}

void StackFrame::feq(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    // if(tmp->type != this->stack->tail->type)
    //     throw TypeMisMatch(_LINE_);

    // if(tmp->type != FLOAT_TYPE)
    //     throw TypeMisMatch(_LINE_);

    float a = this->stack->tail->val;
    float b = tmp->val;

    bool result = a == b;

    this->stack->tail->val = result ? 1 : 0;
    this->stack->tail->type = INTEGER_TYPE;
}

void StackFrame::ineq(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    int a = (int)this->stack->tail->val;
    int b = (int)tmp->val;

    bool result = a != b;

    this->stack->tail->val = result ? 1 : 0;
}

void StackFrame::fneq(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    // if(tmp->type != this->stack->tail->type)
    //     throw TypeMisMatch(_LINE_);

    // if(tmp->type != FLOAT_TYPE)
    //     throw TypeMisMatch(_LINE_);

    float a = this->stack->tail->val;
    float b = tmp->val;

    bool result = a != b;

    this->stack->tail->val = result ? 1 : 0;
    this->stack->tail->type = INTEGER_TYPE;
}

void StackFrame::ilt(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    int a = (int)this->stack->tail->val;
    int b = (int)tmp->val;

    bool result = a < b;

    this->stack->tail->val = result ? 1 : 0;
}

void StackFrame::flt(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    // if(tmp->type != this->stack->tail->type)
    //     throw TypeMisMatch(_LINE_);

    // if(tmp->type != FLOAT_TYPE)
    //     throw TypeMisMatch(_LINE_);

    float a = this->stack->tail->val;
    float b = tmp->val;

    bool result = a < b;

    this->stack->tail->val = result ? 1 : 0;
    this->stack->tail->type = INTEGER_TYPE;
}

void StackFrame::igt(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    if(tmp->type != this->stack->tail->type)
        throw TypeMisMatch(_LINE_);

    if(tmp->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    int a = (int)this->stack->tail->val;
    int b = (int)tmp->val;

    bool result = a > b;

    this->stack->tail->val = result ? 1 : 0;
}

void StackFrame::fgt(){
    if(this->stack->size < 2)
        throw StackEmpty(_LINE_);

    Node* tmp = this->stack->pop();

    // if(tmp->type != this->stack->tail->type)
    //     throw TypeMisMatch(_LINE_);

    // if(tmp->type != FLOAT_TYPE)
    //     throw TypeMisMatch(_LINE_);

    float a = this->stack->tail->val;
    float b = tmp->val;

    bool result = a > b;

    this->stack->tail->val = result ? 1 : 0;
    this->stack->tail->type = INTEGER_TYPE;
}

void StackFrame::ibnot(){
    if(this->stack->size == 0)
        throw StackEmpty(_LINE_);

    if(this->stack->tail->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    int a = (int)this->stack->tail->val;

    bool result = a == 0;

    this->stack->tail->val = result ? 1 : 0;
}

void StackFrame::i2f(){
    if(this->stack->size == 0)
        throw StackEmpty(_LINE_);

    if(this->stack->tail->type != INTEGER_TYPE)
        throw TypeMisMatch(_LINE_);

    this->stack->tail->val = (float)this->stack->tail->val;
    this->stack->tail->type = FLOAT_TYPE;
}

void StackFrame::f2i(){
    if(this->stack->size == 0)
        throw StackEmpty(_LINE_);

    if(this->stack->tail->type != FLOAT_TYPE)
        throw TypeMisMatch(_LINE_);

    this->stack->tail->val = (int)this->stack->tail->val;
    this->stack->tail->type = INTEGER_TYPE;
}

void StackFrame::top(){
    Node* tmp = this->stack->top();
    if(tmp == nullptr) 
        throw StackEmpty(_LINE_);
        
    if(tmp->type == INTEGER_TYPE)
        cout << (int)tmp->val << "\n";
    else 
        cout << tmp->val << "\n";
}

void StackFrame::val(int index){

    if( this->memory->size == 0 ) 
        throw UndefinedVariable(_LINE_);

    if(index < 0 || index >= this->memory->maxSize - 1){
        throw ArrayOutOfRange(_LINE_);
    }

    Node* tmp = this->memory->findIndex(index);

    if(tmp->type == INTEGER_TYPE) 
        cout << (int)tmp->val << "\n";
    else
        cout << tmp->val << "\n";

}
void StackFrame::run(string filename) {
    _LINE_ = 1;
    std::fstream file(filename, std::ios::in); // Open the file for reading
    
    if (!file.is_open()) { return; }

    std::string javmCode;
    float value = 0.0f;
    
    while (file >> javmCode) { // Read each "word" (token) from the file
        // Create Node objects based on javmCode and value
        // cout << javmCode << "\n";

        if (javmCode == "iconst" || javmCode == "fconst") {
            
            file >> value; 
            this->varConst(javmCode, value);

        } else if(javmCode == "istore"){
            
            file >> value; 
            this->istore((int)value);

        } else if(javmCode == "fstore"){
            
            file >> value; 
            this->fstore((int)value);

        } else if(javmCode == "iload"){
            
            file >> value; 
            this->iload((int)value);

        } else if(javmCode == "fload"){
            
            file >> value; 
            this->fload((int)value);

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

        } else if( javmCode == "ineg" || javmCode == "fneg" ){

            this->neg(javmCode);

        } else if( javmCode == "iand" ){

            this->iand();

        } else if( javmCode == "ior" ){

            this->ior();

        } else if( javmCode == "ieq" ){

            this->ieq();

        } else if( javmCode == "feq" ){

            this->feq();

        } else if( javmCode == "ineq" ){

            this->ineq();

        } else if( javmCode == "fneq" ){

            this->fneq();

        } else if( javmCode == "ilt" ){

            this->ilt();

        } else if( javmCode == "flt" ){

            this->flt();

        } else if( javmCode == "igt" ){

            this->igt();

        } else if( javmCode == "fgt" ){

            this->fgt();

        } else if( javmCode == "ibnot" ){

            this->ibnot();

        } else if( javmCode == "i2f" ){

            this->i2f();

        } else if( javmCode == "f2i" ){

            this->f2i();

        } else if(javmCode == "top") {
            
            this->top();

        } else if(javmCode == "val") {
            file >> value; 
            this->val((int)value);
        }

        // this->print();
        _LINE_++;
    }

    file.close(); // Close the file
}