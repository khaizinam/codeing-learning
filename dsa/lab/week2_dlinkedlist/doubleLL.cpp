
#include <iostream>
#include <fstream>
#include "doubleLL.h"

using namespace std;


template <class T>
void DLinkedList<T>::add(const T& e) {
    /* Insert an element into the end of the list. */
    Node* tmp = new Node(e);
    if(this->count == 0){
        this->head = tmp;
        this->tail = tmp;
    } else {
        this->tail->next = tmp;
        tmp->previous = this->tail;
        this->tail = tmp;
    }
    this->count++;
}

template<class T>
void DLinkedList<T>::add(int index, const T& e) {
    /* Insert an element into the list at given index. */ 
    if(index < 0 || (this->count > 0 && index > this->count))
        return;

    if(index == this->count || (index == 0 && this->count == 0)){
        this->add(e);
        return;
    }
    
    Node* tmp = new Node(e);

    if(index == 0){
        this->head->previous = tmp;
        tmp->next = this->head;
        this->head = tmp;
        this->count++;
        return;
    }

    
    Node* prev = this->head;
    for (int i = 0; i < index - 1; i++)
	    prev = prev->next;

	tmp->next = prev->next;
	prev->next = tmp;
	tmp->previous = prev;
	this->count++;
}

template<class T>
int DLinkedList<T>::size() {
    /* Return the length (size) of list */ 
    return this->count;
}


template<class T>
T DLinkedList<T>::get(int index) {
    /* Give the data of the element at given index in the list. */
    if(index < 0 || index > this->count) return -1;
    
    Node* tmp = this->head;
    
    for(int i = 0; i < index; i++)
        tmp = tmp->next;
    
    return tmp->data;
}

template <class T>
void DLinkedList<T>::set(int index, const T& e) {
    /* Assign new value for element at given index in the list */
    if(index < 0 || index > this->count) return;
    
    Node* tmp = this->head;
    
    for(int i = 0; i < index; i++)
        tmp = tmp->next;
    
    tmp->data = e;
}

template<class T>
bool DLinkedList<T>::empty() {
    /* Check if the list is empty or not. */
    return this->count == 0;
}

template<class T>
int DLinkedList<T>::indexOf(const T& item) {
    /* Return the first index wheter item appears in list, otherwise return -1 */
    Node* tmp = this->head;
    
    for(int i = 0; i < this->count; i++){
        if(tmp->data == item) return i;
        tmp = tmp->next;
    }
    
    return -1;
}

template<class T>
bool DLinkedList<T>::contains(const T& item) {
    /* Check if item appears in the list */
     Node* tmp = this->head;
    
    for(int i = 0; i < this->count; i++){
        if(tmp->data == item) return true;
        tmp = tmp->next;
    }
    
    return false;
}

int main(){
    DLinkedList<int> list;
    int size = 10;
    int value[] = {2,5,6,3,67,332,43,1,0,9};
    for(int idx=0; idx < size; idx++){
        list.add(idx);
    }
    for(int idx=0; idx < size; idx++){
        list.set(idx, value[idx]);
    }
    // cout << list.toString();
}