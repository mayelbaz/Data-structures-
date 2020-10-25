
#ifndef RANKTREE_H
#define RANKTREE_H

#include <iostream>
#include <algorithm>
#include "Execptions.h"

using namespace std;
using namespace DS::Tree;

template <typename T>
class RankTree {
public:
    class Node {
        T data; //the data (of a type)
        int node_height;
        Node *right_son; //the node's right son.
        Node *left_son; //the node's left son.
        Node *father; //the node's father. if node is the root, is null.
        int sum;
        int value;
        int size_of_sub_tree;


        Node(const T &data, int value) :
                data(data),
                node_height(0),
                right_son(NULL),
                left_son(NULL),
                father(NULL),
                sum(value),
                value(value),
                size_of_sub_tree(1){};

        //Tree needs access to private fields.
        friend class RankTree<T>;

        ~Node() {}

    public:

        //default assignment operand.
        Node &operator=(const Node &node){
            this->data=node.data;
            this->father=node.father;
            this->left_son=node.left_son;
            this->right_son=node.right_son;
            this->node_height=node.node_height;
            return *this;
        }

        void Print() const{
            std::cout << data << std::endl;
            std::cout << node_height << std::endl;
            std::cout << "" << std::endl;
        }

        T* getNodeData(){
            return &this->data;
        }

    };

private:
    int size; //the number of valid nodes in the tree.
    Node  *root; //the first node in the tree.int height;



    //static const int HEAD_INDEX = 0; //C++ NULL

public:


    /*!
     * Tree c'tor. creates an empty Tree.
     */
    RankTree();

    /*!
     * copy c'tor. constructs a new Tree with given Tree params.
     * @param Tree - the Tree to copy.
     */
    // Tree(const Tree &tree);

    /*!
     * a Tree d'to. deletes all the nodes of a given Tree.
     */
    ~RankTree() {}

    void deleteNode(Node *node){
        if( node ){
            if(node->right_son)
                deleteNode(node->right_son);
            if(node->left_son)
                deleteNode(node->left_son);
            delete (node);
        }
    }

    void deleteRankTree() {
        if( root){
            deleteNode(root);
        }
        this->size = 0;
        this->root=NULL;
    }

    /*!
     * an assignment operator
     * @param Tree - the Tree to be assigned.
     * @return ref to a new Tree that contains the assigned values.
     */
    RankTree &operator=(const RankTree &tree);

    int height(RankTree<T>::Node *node);

    Node* getRoot(){
        return this->root;
    }

    int getBalanceFactor(RankTree<T>::Node *node);

    void updateHeight(Node *node);

    void updateRanksUpToRoot(Node *node){
        if(!node){
            return;
        }
        node->size_of_sub_tree = 1;
        node->sum = node->value;
        if (node->right_son){
            node->size_of_sub_tree += node->right_son->size_of_sub_tree;
            node->sum += node->right_son->sum;
        }
        if (node->left_son){
            node->size_of_sub_tree += node->left_son->size_of_sub_tree;
            node->sum += node->left_son->sum;
        }
        updateRanksUpToRoot(node->father);
    }

    void updateRanks(Node *node){
        if(!node){
            return;
        }
        node->size_of_sub_tree = 1;
        node->sum = node->value;
        if (node->right_son){
            node->size_of_sub_tree += node->right_son->size_of_sub_tree;
            node->sum += node->right_son->sum;
        }
        if (node->left_son){
            node->size_of_sub_tree += node->left_son->size_of_sub_tree;
            node->sum += node->left_son->sum;
        }
    }

    template <typename Compare>
    Node* find(const T& data, Node *root, const Compare &compare){
        if (!root){
            throw TreeNodeDoesNotExit();
        }
        if(compare(root->data, data ) == 0){
            return root;
        } else if (compare(root->data, data ) < 0 ){
            return find(data, root->right_son, compare);
        }
        return find(data, root->left_son, compare);
    }


    int getTopK(Node* node, int k){
        if(k == 0){
            return k;
        }

        if (node->right_son && node->right_son->size_of_sub_tree == k-1){
            return node->value + node->right_son->sum;
        }
        if(node->right_son && node->right_son->size_of_sub_tree == k){
            return node->right_son->sum;
        } else if (node->right_son && node->right_son->size_of_sub_tree > k) {
            return getTopK(node->right_son, k);
        } else if (node->left_son){
            if (!node->right_son){
                return node->value + getTopK(node->left_son, k-1);
            } else {
                return node->value + node->right_son->sum + getTopK(node->left_son, k-1-node->right_son->size_of_sub_tree);
            }
        } else {
            return node->value;
        }
    }

    Node rr_rotation(Node *node){
        rotateLeft(node);
        return *node;
    }

    Node rl_rotation(Node *node){
        rotateRight(node->right_son);
        rotateLeft(node);
        return *node;
    }

    Node ll_rotation(Node *node){
        rotateRight(node);
        return *node;
    }

    Node lr_rotation(Node *node) {
        rotateLeft(node->left_son);
        rotateRight(node);
        return *node;
    }

    /*!
     * return the number of items in the linked Tree.
     * @return - the int number of items.
     */
    int getSize() const;

    void balance(Node *root);

    /*!
     * function adds a new item to the Tree with the data value.
     * inserts at the Tree's end.
     * @param data - the new item's data.
     */
    template <typename Compare>
    void insert(const T &data, Node *root, const Compare &compare, int value){
        if (this->root == NULL){ //if tree is empty
            this->root = new Node(data, value);
            if (!this->root){
                throw TreeMemoryProblemException();
            }
            this->root->right_son=NULL;
            this->root->left_son=NULL;
            this->root->father=NULL;
            this->root->node_height=0;
            //this->root;
            this->size++;
            root = this->root;

        } else if (compare(root->data, data) == 0){
            throw TreeDataAlreadyExists();

        }else if (compare(root->data, data) > 0)
        {
            if(root->left_son){
                insert(data, root->left_son, compare, value);
            } else {
                //Node *newnode = new Node(data);
                root->left_son = new Node(data, value);
                root->left_son->father = root;
                this->size++;
                updateRanksUpToRoot(root->left_son);
                if (root->node_height == 0){
                    Node *Iterator = root;
                    while (Iterator->father != NULL){
                        updateHeight(Iterator);
                        Iterator=Iterator->father;
                    }
                    updateHeight(this->root);
                }
            }
        }  else {
            if(root->right_son){
                insert(data, root->right_son, compare, value);
            } else {
                //Node *newnode = new Node(data);
                root->right_son = new Node(data, value);
                root->right_son->father = root;
                this->size++;
                updateRanksUpToRoot(root->right_son);
                if (root->node_height == 0){
                    Node *Iterator = root;
                    while (Iterator->father != NULL){
                        updateHeight(Iterator);
                        Iterator=Iterator->father;
                    }
                    updateHeight(this->root);
                }
            }
        }
        //this->size++;
        balance(root);
    }

    template <typename Compare>
    void remove(const T& data, const Compare &compare){
        Node *node;
        try {
            node = find(data, this->root, compare); /////////////////////////////////////
        } catch (TreeNodeDoesNotExit){
            throw TreeNodeDoesNotExit();
        }

        removeThis(node, compare);
    }

    template <typename Compare>
    void removeThis(Node *node, const Compare &compare){
        //node has NO sons:
        if(height(node) == 0){
            if(node->father){
                this->size--;
                if(node->father->left_son == node){
                    Node* daddy = node->father;
                    daddy->left_son = NULL;
                    // node->father->left_son = NULL;
                } else {
                    Node* daddy = node->father;
                    daddy->right_son = NULL;
                    //node->father->right_son = NULL;
                }
                Node *Iterator = node;
                while (Iterator->father != NULL){
                    updateHeight(Iterator);
                    balance(Iterator);
                    Iterator=Iterator->father;
                }
                updateRanksUpToRoot(node->father);
                updateHeight(this->root);
                balance(this->root);
            } else { // node is the root
                this->root = NULL;
                this->size = 0;
            }
            delete(node);
            return;
        }
        //check if node is the left son of his father
        bool left = false;
        bool isRoot = false;
        if (!node->father){
            isRoot = true;
        }
        if(!isRoot){
            if(node->father->left_son == node){
                left = true;
            }
        }
        //node has ONE son:
        if (node->left_son == NULL || node->right_son == NULL){
            Node* daddy = node->father;
            if (node->left_son == NULL ){
                if(left){
                    daddy->left_son = node->right_son;
                    node->right_son->father = daddy;
                } else if (!isRoot){
                    daddy->right_son = node->right_son;
                    node->right_son->father = daddy;
                } else {
                    this->root = node->right_son;
                    this->root->father = NULL;
                }

            } else if (node->right_son == NULL){
                if(left){
                    daddy->left_son = node->left_son;
                    node->left_son->father = daddy;
                } else if (!isRoot){
                    daddy->right_son = node->left_son;
                    node->left_son->father = daddy;
                } else {
                    this->root = node->left_son;
                    this->root->father = NULL;
                }

            }
            this->size--;
            if (!isRoot){
                updateHeight(node->father);
                updateRanksUpToRoot(node->father);
                balance(node->father);
            }
            delete(node);
            return;
        }
            //node has TWO sons:
        else if (node->left_son && node->right_son){
            Node *temp = findClosestMin(node);
            changePlaces(node, temp);
            this->size--;
            if(isRoot){
                balance(this->getRoot());
            }
            delete(node);
            if(!isRoot){
                updateHeight(temp->father);
                //updateRanksUpToRoot(temp->father);
            }
            return;
        }
    }

    void changePlaces(Node* to_remove, Node* new_place){
        Node* other_father = new_place->father;
        Node* other_right_son = new_place->right_son;
        //Node* other_left_son = new_place->left_son;

        //new_place->father = NULL;
        new_place->left_son = NULL;
        new_place->right_son = NULL;

        Node* my_father = to_remove->father;
        Node* my_right_son = to_remove->right_son;
        Node* my_left_son = to_remove->left_son;

        to_remove->father = NULL;
        to_remove->right_son = NULL;
        to_remove->left_son = NULL;

        new_place->father = my_father;
        if(my_father == NULL){
            this->root = new_place;
            if(my_right_son == new_place){
                new_place->right_son = other_right_son;
                other_father->left_son = NULL;
            } else {
                new_place->right_son = my_right_son;
                other_father->left_son = other_right_son;
                if (my_right_son){
                    my_right_son->father = new_place;
                }
                if(other_right_son){
                    other_right_son->father = other_father;
                }
            }

        } else {
            if(my_father->left_son == to_remove){
                my_father->left_son = new_place;
            } else {
                my_father->right_son = new_place;
            }
            if(my_right_son == new_place){
                new_place->right_son = other_right_son;
                other_father->left_son = NULL;
            } else {
                new_place->right_son = my_right_son;
                other_father->left_son = other_right_son;
                if (my_right_son){
                    my_right_son->father = new_place;
                }
                if(other_right_son){
                    other_right_son->father = other_father;
                }
            }
        }
        /*if(other_right_son){
            other_right_son->father = other_father;
        }*/
        if (my_left_son){
            my_left_son->father = new_place;
        }
        /*if (my_right_son){
            my_right_son->father = new_place;
        }*/
        new_place->left_son = my_left_son;
        int  temp = to_remove->node_height;
        to_remove->node_height = new_place->node_height;
        new_place->node_height = temp;
        updateRanksUpToRoot(new_place);
        updateHeight(other_father);
        updateHeight(new_place);
        balance(other_father);
    }


    void rotateRight(Node *root);

    void rotateLeft(Node *root);

    Node *findClosestMin(Node *node){
        if (!node){
            throw TreeInvalidInput();
        }
        Node *min = node->right_son;
        while (min->left_son){
            min = min->left_son;
        }
        return min;
    }

    bool operator==(const RankTree &tree) const{
        return this->size == tree.size;
    }

    bool operator!=(const RankTree &tree) const{
        return this->size != tree.size;
    }

    bool operator>(const RankTree &tree) const{
        return this->size > tree.size;
    }

    bool operator<(const RankTree &tree) const{
        return this->size < tree.size;
    }

    void PreOrder (Node *root) const;

    template <typename Action>
    void InOrder (Node *root, Action action) const;

    void InOrderPrint(Node *root){
        if (root){
            RankTree<T>::InOrderPrint(root->left_son);
            root->Print();
            RankTree<T>::InOrderPrint(root->right_son);
        }
    }

    template <typename Compare, typename Action>
    void PostOrderRemove (Node *root, Action &action, const Compare &compare, int *size){
        if (root){
            PostOrderRemove(root->left_son, action, compare, size);
            PostOrderRemove(root->right_son, action,compare, size);
            if(!action(root->data)){
                //removeThis(root, compare);
                (*size)++;
            }
        }
    }

    void PostOrder (Node *root) const;

    void moveInOrderToArray(T *data_array, int *index, Node *root){
        if (root){
            if(!data_array){
                return;
            }
            moveInOrderToArray(data_array, index, root->left_son);
            data_array[(*index)] = *root->getNodeData();
            (*index)++;
            moveInOrderToArray(data_array, index, root->right_son);
        }
    }

    T* moveInOrderToArrayAux(RankTree *tree){
        T* data_array = (T*)malloc(sizeof(T)*(tree->size));
        if(!data_array){
            throw TreeMemoryProblemException();
        }
        int index = 0;
        moveInOrderToArray(data_array, &index,tree->getRoot());
        return data_array;
    }


};

template <class T>
RankTree<T>::RankTree() :
        size(0),
        root (NULL){} ;




template <class T>
RankTree<T>& RankTree<T>::operator=(const RankTree& tree) {
    if (this == &tree){
        return *this;
    }
    delete(this->root);
    this->root=tree.root;
    this->size=tree.size;
    return *this;
}

template <class T>
int RankTree<T>::getSize() const {
    return this->size;
}


template <class T>
int RankTree<T>::getBalanceFactor(RankTree<T>::Node *node){
    if (!node){
        throw TreeInvalidInput();
    }
    return (height(node->left_son) - height(node->right_son));
}

template <class T>
void RankTree<T>::updateHeight(RankTree<T>::Node *node){
    if (!node){
        throw TreeInvalidInput();
    }
    if(height(node->left_son) > height(node->right_son)){
        node->node_height = height(node->left_son) + 1;
    } else {
        node->node_height = height(node->right_son) + 1;
    }
    //if(node)
}

template <class T>
void RankTree<T>::rotateLeft(RankTree<T>::Node *root){
    if(!root){
        throw TreeInvalidInput();
    }
    Node *newroot = root->right_son;
    root->right_son = NULL;
    root->right_son=newroot->left_son;
    newroot->left_son = root;
    if(root->right_son)
        root->right_son->father=root;              //////////////////////////////////////
    if (root->father == NULL){
        this->root = newroot;
        newroot->father = NULL;
    } else {
        if (root->father->left_son == root){
            root->father->left_son = newroot;
        } else {
            Node* daddy = root->father;
            daddy->right_son = newroot;
            //root->father->right_son = newroot;
        }
        newroot->father = root->father;
    }
    root->father = newroot;
    updateHeight(root);
    updateRanks(root);
    updateHeight(newroot);
    updateRanks(newroot);
    //updateHeight(newroot->father);
}

template <class T>
void RankTree<T>::rotateRight(RankTree<T>::Node *root){
    if(!root){
        throw TreeInvalidInput();
    }
    Node *newroot = root->left_son;
    root->left_son = NULL;
    root->left_son=newroot->right_son;
    newroot->right_son = root;
    if(root->left_son)
        root->left_son->father=root; /////////////////////////////////////
    if (root->father == NULL){
        this->root = newroot;
        newroot->father = NULL;
    } else {
        if (root->father->left_son == root){
            root->father->left_son = newroot;
        } else {
            root->father->right_son = newroot;
        }
        newroot->father = root->father;
    }
    root->father = newroot;
    updateHeight(root);
    updateRanks(root);
    updateHeight(newroot);
    updateRanks(newroot);
}


template <class T>
int RankTree<T>::height(RankTree<T>::Node *node){
    if (!node){
        return -1;
    }
    return node->node_height;
}

template <class T>
void RankTree<T>::balance(RankTree<T>::Node *root){
    if (!root){
        throw TreeInvalidInput();
    }
    int BF = getBalanceFactor(root);
    if (BF >= -1 && BF <= 1)
        return;
    if (BF == 2){
        if (getBalanceFactor(root->left_son) >= 0){
            ll_rotation(root);
        } else if (getBalanceFactor(root->left_son) == -1){
            lr_rotation(root);
        }
        Node *Iterator = root;
        while (Iterator->father != NULL){
            updateHeight(Iterator);
            Iterator=Iterator->father;
        }
        updateHeight(this->root);
        return;
    } else if (BF == -2){
        if (getBalanceFactor(root->right_son) <= 0){
            rr_rotation(root);

        } else if (getBalanceFactor(root->right_son) == 1){
            rl_rotation(root);
        }
        Node *Iterator = root;
        while (Iterator->father != NULL){
            updateHeight(Iterator);
            Iterator=Iterator->father;
        }
        updateHeight(this->root);
        return;
    }
    throw TreeBFProblem();
}

template<class T>
void RankTree<T>::PreOrder (RankTree<T>::Node *root) const{
    if( root ){
        root->Print();
        RankTree<T>::PreOrder(root->left_son);
        RankTree<T>::PreOrder(root->right_son);
    }
}

template <class T>
template <typename Action>
void RankTree<T>::InOrder (RankTree<T>::Node *root, Action action) const{
    if (root){
        RankTree<T>::InOrder(root->left_son, action);
        action(root);
        RankTree<T>::InOrder(root->right_son, action);
    }
}

template <class T>
void RankTree<T>::PostOrder (RankTree<T>::Node *root) const{
    if(root){
        RankTree<T>::PostOrder(root->left_son);
        RankTree<T>::PostOrder(root->right_son);
        root->Print();
    }
}


#endif //HW2_RankTREE_H
