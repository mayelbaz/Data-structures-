#include "ChainHash.h"


ChainHash::ChainHash():
    array_size(0),
    occupied(0),
    lists_array(){}


ChainHash::ChainHash(const ChainHash &CH):
        lists_array(CH.lists_array),
        occupied(CH.occupied),
        array_size(CH.array_size){
    lists_array = new List<Clan>[CH.array_size];
    try {
        for (int i = 0; i < array_size; ++i) {
            lists_array[i] = CH.lists_array[i];
        }
    } catch (bad_alloc& exp){
        delete[] lists_array;
        throw exp;
    }
}

ChainHash::ChainHash(int *items_array, int length):
        array_size(length * 2),
        occupied(0),
        lists_array(NULL){
    if (length <= 0){
        throw ChainHashInvalidInput();
    }
    lists_array = new List<Clan>[length * 2];
    this->lists_array = lists_array;
    try {
        for (int i = 0; i < length; ++i) {
            insert(items_array[i]);
        }
    } catch (bad_alloc& exp){
        delete[] lists_array;
        throw exp;
    } catch (ChainHashInvalidInput& exp) {
        delete[] lists_array;
        throw exp;
    }
}

int ChainHash::getSize(){
    return this->occupied;
}

ChainHash::~ChainHash()  {
    delete[] lists_array;
}

void ChainHash::remove(int data) {
    List<Clan>::Iterator it = lists_array[hash(data)].begin();
    while (it != lists_array[hash(data)].end()){
        if (it.getData().getClanID() == data){
            lists_array[hash(data)].remove(it);
            occupied--;
            return;
        }
        it++;
    }
    throw ChainHashElementNotFound();
}

void ChainHash::insert(int data) {
    try{
        find(data);
    } catch (ChainHashElementNotFound& exp){
        if (occupied == array_size){
            reHash((array_size * 2));
        }
        lists_array[hash(data)].insert(Clan(data));
        occupied++;
        return;
    }
    throw ChainHashElementAlreadyExists();
}

Clan& ChainHash::operator[](int data)  {
    return find(data);
}

Clan& ChainHash::find(int data) {
    List<Clan>::Iterator it = lists_array[hash(data)].begin();
    while (it != lists_array[hash(data)].end()){
        if (it.getData().getClanID() == data){
            return it.getData();
        }
        it++;
    }
    throw ChainHashElementNotFound();
}

ChainHash& ChainHash::operator=(const ChainHash &CH){
    List<Clan>* temp = new List<Clan>[CH.array_size];
    try {
        for (int i = 0; i < CH.array_size; ++i) {
            temp[i] = CH.lists_array[i];
        }
    } catch (bad_alloc& exp) {
        delete[] temp;
        throw exp;
    }
    delete[] this->lists_array;
    this->lists_array = temp;
    this->array_size = CH.array_size;
    this->occupied = CH.occupied;
    return *this;
}

void ChainHash::reHash(int new_size) {
    List<Clan>* temp = new List<Clan>[new_size];
    int curr_size = this->array_size;
    try {
        this->array_size = new_size;
        for (int i = 0; i < curr_size; ++i) {
            for (List<Clan>::Iterator it = lists_array[i].begin(); it != lists_array[i].end() ; it++) {
                Clan& curr = it.getData();
                temp[hash(curr.getClanID())].insert(curr);
            }
        }
    } catch (bad_alloc& exp){
        this->array_size = curr_size;
        delete[] temp;
        throw exp;
    }
    delete[] this->lists_array;
    this->lists_array = temp;
}

int ChainHash::hash(int key) {
    return key % array_size;
}
