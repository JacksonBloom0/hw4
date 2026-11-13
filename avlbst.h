#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    /*
        a
    b       c
  d   e
f


rotateRight(b)
- d's parent becomes b's parent
- b becomes d's right child
- if d already had a right child, it becomes b's left child

d = b->getLeft()
d->setParent(b->getParent)
temp = d->getRight()
d->setRight(b)
b->setLeft(temp)

    */
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);
    void updateAllBalances();
    void fix(AVLNode<Key, Value>* n);

    AVLNode<Key, Value>* getRoot();
    void setRoot();



};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if (getRoot() == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    if (AVLNode<Key, Value>* ptr = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first))) {
        ptr->setValue(new_item.second);
        return;
    }
    AVLNode<Key, Value>* curr = this->getRoot();
    const Key key = new_item.first;

    bool is_left_child = false;
    // std::cout << "in insert... tracing" << std::endl;
    while(curr != nullptr) {
        if (key >= curr->getKey()) {
            if (curr->getRight()) {
                curr = curr->getRight();
            }
            else {
                AVLNode<Key, Value>* new_leaf = new AVLNode<Key, Value>(key, new_item.second, curr);
                curr->setRight(new_leaf);
                is_left_child = false;
                break;
            }
        }
        else {
            if (curr->getLeft()) {
                curr = curr->getLeft();
            }
            else {
                AVLNode<Key, Value>* new_leaf = new AVLNode<Key, Value>(key, new_item.second, curr);
                curr->setLeft(new_leaf);
                is_left_child = true;
                break;
            }
        }
    }

    // std::cout << "in insert... checking fix" << std::endl;
    AVLNode<Key, Value>* to_fix = nullptr;
    curr->updateBalance(1 - 2 * is_left_child);
    while (curr->getParent() && curr->getBalance() != 0) {
        is_left_child = (curr == curr->getParent()->getLeft());
        curr = curr->getParent();
            curr->updateBalance(1 - 2 * is_left_child);
            // std::cout << "curr balance: " << static_cast<int>(curr->getBalance()) << std::endl;
            if (std::abs(curr->getBalance()) > 1) {
                // std::cout << "found imbalance: " << static_cast<int>(curr->getBalance()) << std::endl;
                to_fix = curr;
            }
        
    }
    // this->print();
    fix(to_fix);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* ptr = static_cast<AVLNode<Key, Value>* >(this->internalFind(key));
    if (!ptr) {
        return;
    }
    // std::cout << "removing " << static_cast<char>(key) << std::endl;
    // std::cout << "before tree: "  << std::endl;
    // this->print();
    AVLNode<Key, Value>* l = ptr->getLeft();
    AVLNode<Key, Value>* r = ptr->getRight();

    // If no children, do no swaps

    if (r && l) { // both, swap with pred
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>* >(this->predecessor(ptr));
        nodeSwap(ptr, pred);
    }

    else if (l) { // one child swap
        nodeSwap(ptr, l);
    }
    else if (r) { // one child swap
        nodeSwap(ptr, r);
    }

    bool is_left_child = false;
    AVLNode<Key, Value>* curr = ptr->getParent();
    if (!curr) {
        delete ptr;
        ptr = nullptr;
        return;
    }
    
    if (ptr->getParent()) { // if parent, set parent's child to null before deletion
        if (ptr->getParent()->getLeft() == ptr) {
            is_left_child = true;
            ptr->getParent()->setLeft(nullptr);
        }
        else if (ptr->getParent()->getRight() == ptr) {
            ptr->getParent()->setRight(nullptr);
        }
    }
    if (AVLNode<Key, Value>* p_child = ptr->getLeft()) {
        p_child->setParent(ptr->getParent());
    }
    else if (AVLNode<Key, Value>* p_child = ptr->getRight()) {
        p_child->setParent(ptr->getParent());
    }
    delete ptr;
    ptr = nullptr;

    // std::cout << "checking balances " << std::endl;
    AVLNode<Key, Value>* to_fix = nullptr;
    // auto pre_balance = curr->getBalance(); // getting balance before updating, if pre-balanced, no change to parent's balance and no need to further update
    // curr->updateBalance(-1 + 2 * is_left_child); // updating balance of parent of node being removed
    auto pre_balance = curr->getBalance();
    curr->updateBalance(-1 + 2 * is_left_child);
    // std::cout << "balance at first node " << static_cast<char>(curr->getKey()) << ", :" << static_cast<int>(curr->getBalance()) << std::endl;
    while (curr->getParent() && pre_balance != 0) {
        // std::cout << "pre_balance: " << static_cast<int>(pre_balance) << std::endl;
        is_left_child = (curr == curr->getParent()->getLeft());
        curr = curr->getParent();
        pre_balance = curr->getBalance();
        curr->updateBalance(-1 + 2 * is_left_child);
        // std::cout << "updated balance: " << static_cast<int>(pre_balance) << std::endl;
        if (std::abs(curr->getBalance()) > 1) {
            to_fix = curr;
            break;
        }

        // is_left_child = (curr == curr->getParent()->getLeft()); // getting side before moving onto parent

        // curr = curr->getParent();
        // pre_balance = curr->getBalance();
        // is_left_child = (curr == curr->getParent()->getLeft());
        // curr->updateBalance(-1 + 2 * is_left_child);
        // // CHECK
        
    }
    if (!curr->getParent()) {
        curr->updateBalance(-1 + 2 * is_left_child);
        if (std::abs(curr->getBalance()) > 1) {
            to_fix = curr;
        }
    }
    
    
    // curr == root || curr->balance < -1 || > 1
    fix(to_fix);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getRoot() {
    return static_cast<AVLNode<Key, Value>*>(this->root_);
}

template<class Key, class Value>
void AVLTree<Key, Value>::fix(AVLNode<Key, Value>* n) {
    /*
        P             P
      n      ->    a
         a       n

    Zigzag (-2, 1) (left right) (rotateLeft(n)) (rotateRight(P))
    Zigzag(2, -1) (right, left)
    Zigzig (-2, -1) rotateRight(P)
    zigzig (2, 1) rotateLeft(P)

    */
    if (n == nullptr) {
        return;
    }
    if (n->getBalance() < 0) {
        if (n->getLeft()->getBalance() < 0) { // zig zig left
            rotateRight(n);
        }
        else { // zig zag left-right
            rotateLeft(n->getLeft());
            rotateRight(n);
        }
    }
    else {
        if (n->getRight()->getBalance() > 0) { // zig zig right
            // std::cout << "in zig zig right" << std::endl;
            rotateLeft(n);
            
        }
        else { // zig zag right-left
            rotateRight(n->getRight());
            rotateLeft(n);
            
        }
    }
    
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n) {
    /*
            P
        n
     a     b
   .   t
    
    */
//    std::cout << "before fix" << std::endl;
    // this->print();
    // std::cout << "rotating right about " << n->getKey() << std::endl;
    AVLNode<Key, Value>* a = n->getLeft();
    if (!a) {
        // std::cout << "nothing to rotate" << std::endl;
        return;
    }

    AVLNode<Key, Value>* parent = n->getParent();
    a->setParent(parent);
    if (parent) {
        if (parent->getRight() == n) {
            parent->setRight(a);
        } else {
            parent->setLeft(a);
        }
    }
    else {
        this->root_ = a;
    }
    AVLNode<Key, Value>* temp = a->getRight();
    a->setRight(n);
    n->setParent(a);
    n->setLeft(temp);



    int oldNBF = n->getBalance();
    int oldABF = a->getBalance();

    n->setBalance(oldNBF + 1 - std::min(oldABF, 0));
    a->setBalance(oldABF + 1 + std::max(static_cast<int>(n->getBalance()), 0));

    // std::cout << "new balances: " << static_cast<int>(n->getBalance()) << ", " << static_cast<int>(a->getBalance()) << std::endl;
    // std::cout << "after fix" << std::endl;
    // this->print();
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n) {
    /*
        P             P
      n      ->    a
    b   a       n
       d      b   d

    */
//    std::cout << "before fix" << std::endl;
//     this->print();
//     std::cout << "rotating left about " << n->getKey() << std::endl;
    AVLNode<Key, Value>* a = n->getRight();
    if (!a) {
        // std::cout << "nothing to rotate" << std::endl;
        return;
    }

    AVLNode<Key, Value>* parent = n->getParent();
    a->setParent(parent);
    if (parent) {
        if (parent->getLeft() == n) {
            parent->setLeft(a);
        } else {
            parent->setRight(a);
        }
    }
    else {
        this->root_ = a;
    }

    AVLNode<Key, Value>* temp = a->getLeft();
    a->setLeft(n);
    n->setParent(a);
    n->setRight(temp);
    if (temp) {
        temp->setParent(n);
    }


    int oldNBF = n->getBalance();
    int oldABF = a->getBalance();

    n->setBalance(oldNBF - 1 - std::max(oldABF, 0));

    a->setBalance(oldABF - 1 + std::min(static_cast<int>(n->getBalance()), 0));
    // std::cout << "new balances: " << static_cast<int>(n->getBalance()) << ", " << static_cast<int>(a->getBalance()) << std::endl;
    // std::cout << "after fix" << std::endl;
    // this->print();
}




#endif
