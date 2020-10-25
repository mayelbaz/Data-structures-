#ifndef EX4_WET_LIST_H
#define EX4_WET_LIST_H

#include <iostream>
#include <assert.h>
#include <algorithm>
#include "Execptions.h"

using namespace std;
using namespace DS::List; //needed for exeptions.


template <typename T>
class List {
    class Node {
        T data; //the data (of a type)
        Node *next; //the next link in the list.
        Node *previous; //the prev. link in the list.

        /*!
         * the basic c'tor. constructs a node that contains the basic data.
         * @param data - the actuall type & data of the specific link in the
         * list
         */
        Node(const T &data) :
                data(data),
                next(NULL),
                previous(NULL) {};

        //List needs access to private fields.
        friend class List<T>;

        //Iterator needs access to private fields.
        friend class Iterator;

    public:

        //default assignment operand.
        //Node &operator=(const Node &node) = default;

    };

    int size; //the number of valid nodes in the list.
    Node  *list_head; //the first node in the list.
    Node  *list_end; //NULL node, signefy the list's end.



    static const int HEAD_INDEX = 0; //C++ NULL

public:


    /*!
     * list c'tor. creates an empty list.
     */
    List();

    /*!
     * copy c'tor. constructs a new list with given list params.
     * @param list - the list to copy.
     */
    List(const List &list);

    /*!
     * a list d'to. deletes all the nodes of a given list.
     */
    ~List();

    /*!
     * an assignment operator
     * @param list - the list to be assigned.
     * @return ref to a new list that contains the assigned values.
     */
    List &operator=(const List &list);

    /*!
     * return the number of items in the linked list.
     * @return - the int number of items.
     */
    int getSize() const;

    /*!
     * function adds a new item to the list with the data value.
     * inserts at the list's end.
     * @param data - the new item's data.
     */
    void insert(const T &data);

    /*!
     * functions sorts the list according to the given Compare function.
     * @tparam Compare - function object. returns true if first param is before
     *                   second param.
     * @param compare - the compare function
     */
    template <typename Compare>
    void sort(const Compare &compare);

    /*!
     * checks if two lists are equal.
     * @param list - the right hand list to compare to.
     * @return true if both lists contains the same items at the same order.
     */
    bool operator==(const List &list) const;

    /*!
     * check if two lists are NOT equal/
     * @param list - the right hand list to compare to.
     * @return the ! of ==.
     */
    bool operator!=(const List &list) const;

    /*!
     * class iterator. contains a pointer to current node & a pointer to the
     * list for compare porpeses.
     */
    class Iterator {
        Node *current; //points to the current link in the list.
        const List<T> *list; //points to the list the iterator belongs to.

        /*!
         * the basic c'tor. constructs a new iterator that points to the given
         * node.
         * @param list - the list that the iterator points to
         * @param node - the lists node that the iterator points to.
         */
        Iterator(const List<T> *list, Node *node) :
                current(node),
                list(list) {}

        //list needs access to private fields.
        friend class List<T>;

    public:

        //class Node;

        T& getData() const{
            return this->current->data;
        }
        /*!
         * Dereference. this operator returns the data (of T type) that the
         * iterator points to.
         * @return - the data field of the current node.
         */
        T& operator*() const;

        /*!
         * Advances the iterator by one node (by action end, iterator points to
         * the next node of the list)
         * @return - the next node in the list.
         */
        Iterator &operator++();

        /*!
         * Advances the iterator by one node (by action end, iterator  points to
         * the next node of the list, but return val does not reflect it until
         * line end)
         * @return - pre-action iterator.
         */
        Iterator operator++(int);

        /*!
         * Retracts the iterator by one node (by action end, iterator points to
         * the previous node of the list)
         * @return - the previous node in the list.
         */
        Iterator &operator--();

        /*!
         * Retracts the iterator by one node (by action end, iterator  points to
         * the previous node of the list, but return val does not reflect it
         * until line end)
         * @return - pre-action iterator.
         */
        Iterator operator--(int);

        /*!
         * compare operator. checks if the iterator are equal. two iterators
         * would be considered equal if both are pointing to the same node
         * (or to list end) at the same list.
         * @param iterator - the right hand operand to compare.
         * @return true  - if the iterators are equal,
         *          false - otherwise.
         */
        bool operator==(const Iterator &iterator) const;

        /*!
         * compare operator. checks if the iterator are NOT equal.
         * @param iterator - the right hand operand to compare.
         * @return the not of the == operator result.
         */
        bool operator!=(const Iterator &iterator) const;

        //default copy operand. fields are basic pointers.
        //Iterator(const Iterator &iterator) = default;

        //default assignment operand.
        //Iterator &operator=(const Iterator &iterator) = default;

    };

    /*!
     * function adds a new item to the list with the data value.
     * inserts before given iterator.
     * @param data - the new item's data.
     * @param iterator - the iterator to insert begore.
     *
     * @exception ElementNotFound in case that the given iterator is of another
     *              list.
     */
    void insert(const T &data, Iterator iterator);

    /*!
     * function removes an item off the list.
     * @param iterator - points to the item to remove.
     *
     * @exception ElementNotFound in case that the given iterator is of another
     *              list, or iterator points to the list's end.
     */
    void remove(Iterator iterator);

    /*!
     * return the Iterator to the list head.
     * @return the list head node data (the first item on the list.
     */
    Iterator begin() const;

    /*!
     * returns the iterator the list end.
     * iterator will point to NULL by the function's end.
     * @return NULL iterator
     */
    Iterator end() const;

    /*!
     * function finds an item in the list the fits the Predicate critiria.
     * @tparam Predicate - function object. returns true if an item fits.
     * @param predicate - the function object.
     * @return the iterator value of the first item that fits.
     * if no items fit, returns the list end.
     */
    template <typename Predicate>
    Iterator find(const Predicate &predicate, T data);

};

template <class T>
List<T>::List() :
        size(HEAD_INDEX),
        list_head (NULL),
        list_end (NULL)
/*list_iterator(this, list_head)*/{}


template <class T>
List<T>::List(const List& list) :
        size(HEAD_INDEX),
        list_head(NULL),
        list_end(NULL){
    for(Node *current = list.list_head; current != NULL;
        current = current->next){
        insert(current->data);
    }
}

template <class T>
List<T>::~List() {
    Node *node = list_head;
    while (node != NULL){
        Node *next = node->next;
        delete node;
        node = next;
    }
}

template <class T>
List<T>& List<T>::operator=(const List& list) {
    if (this == &list){
        return *this;
    }
    int length = this->size;
    for (int l = 0; l < length; ++l) {
        this->remove(this->begin());
    }
    //list_head = current;
    for(Node* current = list.list_head; current != list.list_end;
        current = current->next){
        insert(current->data);
    }
    //list_end = current;
    return *this;
}

template <class T>
int List<T>::getSize() const {
    return this->size;
}

template <class T>
typename List<T>::Iterator List<T>::begin() const {
    return Iterator(this, list_head);
}

template <class T>
typename List<T>::Iterator List<T>::end() const {
    return Iterator(this, list_end);
}

template <class T>
void List<T>::insert(const T &data, Iterator iterator) {
    if(iterator.current == list_end){
        insert(data);
        return;
    }
    Node *current = list_head;
    while (current != iterator.current && current != list_end){
        current = current->next;
    }
    if (current == list_end){
        throw ElementNotFound();
    }
    Node *newNode = new Node(data);
    newNode->next = current;
    if(current->previous != NULL){
        Node *previous = current->previous;
        previous->next = newNode;
        newNode->previous = previous;
    } else {
        newNode->next = list_head;
        list_head->previous = newNode;
        list_head = newNode;
        newNode->previous = NULL;
    }
    current->previous = newNode;
    size++;
}

template <class T>
void List<T>::insert(const T &data) {
    Node *newNode = new Node(data);
    if(size == 0){
        list_head = newNode;
        newNode->previous = NULL;
        newNode->next = NULL;
        size++;
        return;
    }
    Node *current = list_head;
    while (current->next != list_end){
        current = current->next;
    }
    current->next = newNode;
    newNode->previous = current;
    size++;
}

template <class T>
void List<T>::remove(Iterator iterator) {
    if (size == 0 || this != iterator.list){
        throw ElementNotFound();
    }
    Node *current = list_head;
    while (current->data != iterator.current->data && current != list_end){
        current = current->next;
    }
    if(current == list_end){
        throw ElementNotFound();
    }
    if(iterator.current->previous != NULL){
        (current->previous)->next = current->next;
    }else {
        list_head = current->next;
    }
    if(iterator.current->next != NULL){
        (current->next)->previous = current->previous;
    }
    delete(current);
    size--;
}

template <typename T>
template <typename Predicate>
typename List<T>::Iterator List<T>::find(const Predicate &predicate, T data){
    for(Node *current = list_head; current != NULL; current = current->next){
        if(predicate(current->data, data)){
            return Iterator(this, current);
        }
    }
    return this->end();
}

template <typename T>
template <typename Compare>
void List<T>::sort(const Compare &compare){
    if(size == 0 || size == 1){ //no sort needed
        return;
    }
    Node *current = list_head;
    List<T> sorted;
    bool added = false;
    sorted.insert(list_head->data);
    current = current->next;
    while (current){
        added = false;
        Node* other = sorted.list_head;
        for (int i = 0; i < sorted.size; ++i) {
            if(compare(current->data, other->data) == true){
                Iterator insert(&sorted, other);
                sorted.insert(current->data, insert);
                added = true;
                break;
            }
            other = other->next;
        }
        if(!added){
            sorted.insert(current->data);
        }
        if(current->next != NULL){
            current = current->next;
        } else {
            break;
        }
    }
    int length = this->size;
    for (int l = 0; l < length; ++l) {
        this->remove(this->begin());
    }
    *this = sorted;
}

template <class T>
bool List<T>::operator==(const List& list) const{
    bool same = (this->size == list.size);
    if (same){
        Node *other = list.list_head;
        Node *current = list_head;
        while (current){
            assert(other != list.list_end); //we checked that the list are
            // of the same size.
            if (other->data != current->data){
                same = false;
                break;
            }
            if(other->next != NULL){
                other = other->next;
                current = current->next;
            } else if (current->next != NULL){
                same = false;
                break;
            } else{
                current = NULL;
            }
        }
    }
    return same;
}

template <class T>
bool List<T>::operator!=(const List& list) const{
    return !(*this == list);
}


template <class T>
T& List<T>::Iterator::operator*() const {
    if(current == NULL){
        throw ElementNotFound();
    }
    return current->data;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    if(current != NULL){
        current = current->next;
    }
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    Iterator result = *this;
    ++*this;
    return result;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
    if(current->previous != NULL){
        current = current->previous;
    }
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    Iterator result = *this;
    --*this;
    return result;
}

template <class T>
bool List<T>::Iterator::operator==(const Iterator& iterator) const{
    if(current == this->list->list_end &&
       iterator.current == iterator.list->list_end){
        return (list == iterator.list);
    } else if (current != this->list->list_end &&
               iterator.current != iterator.list->list_end){
        return (list == iterator.list &&
                current->data == iterator.current->data);
    } else {
        return false;
    }
}

template <class T>
bool List<T>::Iterator::operator!=(const Iterator& iterator) const{
    return !(*this == iterator);
}

#endif