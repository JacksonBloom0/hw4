#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int get_depth(Node * root) {
    if (root == nullptr) {
        return 0;
    }
    int right_depth = 1 + get_depth(root->right);
    int left_depth = 1 + get_depth(root->left);
    // condition ? expression_if_true : expression_if_false;
    return right_depth >= left_depth ? right_depth : left_depth;

}

bool equalPaths(Node * root)
{
    if (root == nullptr) {
        return true;
    }
    if (!root->left) {
      return equalPaths(root->right);
    }
    if (!root->right) {
      return equalPaths(root->left);
    }
    
    if (get_depth(root->right) != get_depth(root->left)) {
      return false;
    }
    return equalPaths(root->right) && equalPaths(root->left);

}