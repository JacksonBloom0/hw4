#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('c',1));
    bt.insert(std::make_pair('d',2));
    bt.insert(std::make_pair('f',-1));
    bt.insert(std::make_pair('b',0));
    bt.insert(std::make_pair('e',9));
    bt.insert(std::make_pair('a',12));
    // bt.insert(std::make_pair('g',7));
    // bt.insert(std::make_pair('z',7));
    bt.print();
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {

        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    // bt.remove('b');
    std::cout << "done" << std::endl;
    bt.print();
    cout << "-------------------------------------------------------------------" << endl << endl << endl;

    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('c',1));
    cout << "1" << endl;
    at.insert(std::make_pair('d',2));
    cout << "2" << endl;
    at.insert(std::make_pair('f',-1));
    cout << "3" << endl;
    at.insert(std::make_pair('b',0));
    cout << "4" << endl;
    at.insert(std::make_pair('e',9));
    cout << "5" << endl;
    // at.print();
    at.insert(std::make_pair('a',12));
    cout << "6" << endl;
    cout << "attempting to print" << endl;
    // at.print();
    at.insert(std::make_pair('g',7));
    at.insert(std::make_pair('z',7));
    // at.print();

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }

    cout << "Erasing b" << endl;
    at.remove('b');
    // at.remove('b');
    at.remove('c');
    at.remove('z');
    at.remove('g');
    at.remove('a');

    at.print();

    return 0;
}
