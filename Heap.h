#ifndef WET2_HEAP_H
#define WET2_HEAP_H

#include <iostream>
#include <algorithm>
#include "Execptions.h"

using namespace std;
using namespace DS::MinHeap;

template <typename T>
class MinHeap {
    T *items;
    int occupied;
    int array_length;

    void moveIntoArray(T* dest, const T* source, int length) {
        for (int i = 1; i <= length ; ++i) {
            dest[i] = source[i];
        }
    }



    void reSize (int new_length){
        T *temp = new T[new_length + 1];
        moveIntoArray(temp, this->items, this->array_length);
        delete[] this->items;
        this->items = temp;
        this->array_length = new_length;
    }

    int getParentIndex(int son) {
        return (son/2);
    }

    int getLeftSonIndex(int father){
        return 2*father;
    }

    int getRightSonIndex(int father){
        return 2*father+1;
    }

    void swap(int son, int father){
        T temp;
        temp = items[son];
        T debug = items[father];
        items[son] = debug;
        items[father] = temp;
    }

    void siftDown(int item_index) {
        int min_index;
        int left_son = getLeftSonIndex(item_index);
        int right_son = getRightSonIndex(item_index);
        while (item_index <= occupied/2){
            if(right_son <= occupied){
                if(items[item_index] < items[left_son] &&
                   items[item_index] < items[right_son]){ //no need to sift
                    break;
                }
                if(items[left_son] < items[right_son]){
                    min_index = left_son;
                } else{
                    min_index = right_son;
                }
            } else {
                if (items[item_index] < items[left_son]) { //no need to sift
                    break;
                }
                min_index = left_son;
            }
            swap(min_index, item_index);
            item_index = min_index;
            left_son = getLeftSonIndex(item_index);
            right_son = getRightSonIndex(item_index);
        }
    }

    void siftUp (int item_index) {
        while (item_index > 1 && items[item_index] < items[item_index/2]){
            int father = getParentIndex(item_index);
            swap(item_index, father);
            item_index /= 2;
        }
    }

public:

    //fony copy c'tor
    MinHeap(const T* items, int length):
            items(NULL), occupied(length), array_length(2*length){
        if(length <= 0){
            throw HeapInvalidInput();
        }
        this->items = new T[1 + (length * 2)];
        for (int i = 0; i < length; ++i) {
            this->items[i+1] = items[i];
        }
        for (int j = length/2; j >=1 ; j--) {
            siftDown(j);
        }
    }

    MinHeap():
        items(NULL),
        occupied(0),
        array_length(0){}



    //d'tor
    ~MinHeap(){
        delete[] items;
    }

    MinHeap(const MinHeap& heap) :
            items(new T[1 + heap.array_length]),
            occupied(heap.occupied),
            array_length(heap.array_length){
        moveIntoArray(items, heap.items, occupied);
    }

    void insert(T data){
        if (occupied == array_length){
            reSize(array_length * 2);
        }
        occupied++;
        items[occupied] = data;
        siftUp(occupied);
    }

    //void printarr()

    T getMin() const {
        if (this->occupied == 0){
            throw HeapEmpty();
        }
        return this->items[1];
    }

    int getSize(){
        return this->occupied;
    }

    void removeMin(){
        if (occupied == 0){
            throw HeapEmpty();
        }
        this->items[1] = this->items[occupied];
        occupied--;
        siftDown(1);
    }


};


#endif //WET2_HEAP_H
