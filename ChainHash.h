#ifndef WET2_CHAINHASH_H
#define WET2_CHAINHASH_H

#include "List.h"
#include "PlayerClan.h"

using namespace DS::ChainHash;

class ChainHash {
    int array_size;
    int occupied;
    List<Clan>* lists_array;

    int hash(int key);

    void reHash(int new_size);

public:

    //ctor

    ChainHash();

    ChainHash& operator=(const ChainHash& CH);

    Clan& find(int data);

    Clan& operator[](int data);

    void insert(int data);

    void remove(int data);

    int getSize();

    ChainHash(int *items_array, int length);

    ~ChainHash();

    ChainHash(const ChainHash& CH);


};


#endif //WET2_CHAINHASH_H
