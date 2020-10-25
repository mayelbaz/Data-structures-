
#ifndef HW2_TREE_H
#define HW2_TREE_H

#include <iostream>
#include <algorithm>
#include "Execptions.h"

using namespace std;
using namespace DS::Tree;

template <typename T>
class Tree {
public:
    class Node {
        T data; //the data (of a type)
        int node_height;
        Node *right_son; //the node's right son.
        Node *left_son; //the node's left son.
        Node *father; //the node's father. if node is the root, is null.


        Node(const T &data) :
                data(data),
                node_height(0),
                right_son(NULL),
                left_son(NULL),
                father(NULL){};

        //Tree needs access to private fields.
        friend class Tree<T>;

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
    Tree();

    /*!
     * copy c'tor. constructs a new Tree with given Tree params.
     * @param Tree - the Tree to copy.
     */
    // Tree(const Tree &tree);

    /*!
     * a Tree d'to. deletes all the nodes of a given Tree.
     */
    ~Tree() {}

    void deleteNode(Node *node){
        if( node ){
            if(node->right_son)
                deleteNode(node->right_son);
            if(node->left_son)
                deleteNode(node->left_son);
            delete (node);
        }
    }

    void deleteTree() {
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
    Tree &operator=(const Tree &tree);

    int height(Tree<T>::Node *node);

    Node* getRoot(){
        return this->root;
    }

    int getBalanceFactor(Tree<T>::Node *node);

    void updateHeight(Node *node);

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
    void insert(const T &data, Node *root, const Compare &compare){
        if (this->root == NULL){ //if tree is empty
            this->root = new Node(data);
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
                insert(data, root->left_son, compare);
            } else {
                //Node *newnode = new Node(data);
                root->left_son = new Node(data);
                root->left_son->father = root;
                this->size++;
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
                insert(data, root->right_son, compare);
            } else {
                //Node *newnode = new Node(data);
                root->right_son = new Node(data);
                root->right_son->father = root;
                this->size++;
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

    bool operator==(const Tree &tree) const{
        return this->size == tree.size;
    }

    bool operator!=(const Tree &tree) const{
        return this->size != tree.size;
    }

    bool operator>(const Tree &tree) const{
        return this->size > tree.size;
    }

    bool operator<(const Tree &tree) const{
        return this->size < tree.size;
    }

    void PreOrder (Node *root) const;

    template <typename Action>
    void InOrder (Node *root, Action action) const;

    void InOrderPrint(Node *root){
        if (root){
            Tree<T>::InOrderPrint(root->left_son);
            root->Print();
            Tree<T>::InOrderPrint(root->right_son);
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

    T* moveInOrderToArrayAux(Tree *tree){
        T* data_array = (T*)malloc(sizeof(T)*(tree->size));
        if(!data_array){
            throw TreeMemoryProblemException();
        }
        int index = 0;
        moveInOrderToArray(data_array, &index,tree->getRoot());
        return data_array;
    }

    template <typename Compare, typename Action>
    void uniteTreesAux( Action &action, Tree<T> &tree, const Compare compare, int *my_size, int *his_size){
        T* my_data = moveInOrderToArrayAux(this);
        Tree *ptr = &tree;
        T* his_data = moveInOrderToArrayAux(ptr);
        T* united_data = (T*)malloc(sizeof(T)*((*my_size) + (*his_size)));
        if(!my_data || !his_data || !united_data){
            throw TreeMemoryProblemException();
        }
        int my_index, his_index, our_index;
        //bool nothing_to_do = false;
        for (my_index=his_index=our_index= 0; (my_index<this->size)&&(his_index<tree.size) ; ) {
            if(compare(my_data[my_index], his_data[his_index])<0){
                if(!action(my_data[my_index])){
                    united_data[our_index] = my_data[my_index];
                    my_index++;
                    our_index++;
                }else {
                    my_index++;
                }
            } else {
                if(!action(his_data[his_index])){
                    united_data[our_index] = his_data[his_index];
                    his_index++;
                    our_index++;
                } else {
                    his_index++;
                }
            }
        }
        for (; my_index < this->size; my_index++) {
            if(!action(my_data[my_index])){
                united_data[our_index] = my_data[my_index];
                our_index++;
            }
        }
        for (; his_index < tree.size; his_index++) {
            if(!action(his_data[his_index])){
                united_data[our_index] = his_data[his_index];
                our_index++;
            }
        }
        free(my_data);
        free(his_data);
        int total_size = (*my_size) + (*his_size);
        tree.deleteTree();
        this->deleteTree();
        this->size = total_size;
        //delete (tree);
        this->root=uniteTrees(united_data, total_size, this->root);
        free(united_data);
    }

    Node* uniteTrees(T* united_data, int total_size, Node *root){
        if(total_size <=0 ){
            return NULL;
        }
        int mid = total_size/2;
        root = new Node(united_data[mid]);
        root->right_son = uniteTrees(united_data+mid+1,total_size-mid-1, root->right_son);
        if(root->right_son){
            root->right_son->father = root;
        }
        root->left_son = uniteTrees(united_data, mid, root->left_son);
        if(root->left_son){
            root->left_son->father = root;
        }
        updateHeight(root);
        return root;
    }

};

template <class T>
Tree<T>::Tree() :
        size(0),
        root (NULL){} ;




template <class T>
Tree<T>& Tree<T>::operator=(const Tree& tree) {
    if (this == &tree){
        return *this;
    }
    delete(this->root);
    this->root=tree.root;
    this->size=tree.size;
    return *this;
}

template <class T>
int Tree<T>::getSize() const {
    return this->size;
}


template <class T>
int Tree<T>::getBalanceFactor(Tree<T>::Node *node){
    if (!node){
        throw TreeInvalidInput();
    }
    return (height(node->left_son) - height(node->right_son));
}

template <class T>
void Tree<T>::updateHeight(Tree<T>::Node *node){
    if (!node){
        throw TreeInvalidInput();
    }
    if(height(node->left_son) > height(node->right_son)){
        node->node_height = height(node->left_son) + 1;
    } else {
        node->node_height = height(node->right_son) + 1;
    }
}

template <class T>
void Tree<T>::rotateLeft(Tree<T>::Node *root){
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
    updateHeight(newroot);
    //updateHeight(newroot->father);
}

template <class T>
void Tree<T>::rotateRight(Tree<T>::Node *root){
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
    updateHeight(newroot);
}


template <class T>
int Tree<T>::height(Tree<T>::Node *node){
    if (!node){
        return -1;
    }
    return node->node_height;
}

template <class T>
void Tree<T>::balance(Tree<T>::Node *root){
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
void Tree<T>::PreOrder (Tree<T>::Node *root) const{
    if( root ){
        root->Print();
        Tree<T>::PreOrder(root->left_son);
        Tree<T>::PreOrder(root->right_son);
    }
}

template <class T>
template <typename Action>
void Tree<T>::InOrder (Tree<T>::Node *root, Action action) const{
    if (root){
        Tree<T>::InOrder(root->left_son, action);
        action(root);
        Tree<T>::InOrder(root->right_son, action);
    }
}

template <class T>
void Tree<T>::PostOrder (Tree<T>::Node *root) const{
    if(root){
        Tree<T>::PostOrder(root->left_son);
        Tree<T>::PostOrder(root->right_son);
        root->Print();
    }
}


#endif //HW2_TREE_H
