#include "BinarySearchTree.h"

BinarySearchTree::~BinarySearchTree() {
    if(root) {
        cout << "delete tree "  << endl;
        delete root;
        root = nullptr;
    }
}

const int & BinarySearchTree::findMin() const { findMin(root); }
const int & BinarySearchTree::findMax() const{  findMax(root); }
bool BinarySearchTree::contains( const int & x) const {return contains(x, root);}


bool BinarySearchTree::isEmpty() const { return root->N <= 0; }
int BinarySearchTree::getSize() const { return root->N; }
void BinarySearchTree::printTree() const {  printTree(root); }

void BinarySearchTree::insert( const int & x) { insert(x, root); }
void BinarySearchTree::remove( const int & x) { remove(x, root); }



void BinarySearchTree::insert(const int & x, BinaryNode * &t) const {
    if (!t) t = new BinaryNode(x, NULL, NULL);
    else if( x < t->element)    insert(x, t->left);
    else if(t->element < x)     insert(x, t->right);
}

void BinarySearchTree::remove(const int & x, BinaryNode * &t) const {
    if (!t) return;
    if (x < t->element) remove(x, t->left);
    else if(x > t->element) remove(x, t->right);
    else if(!(t->left)&&!(t->right)) {
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }else {
        BinaryNode *oldNode = t;
        t = (t->left!=NULL) ? t->left : t->right;
        delete oldNode;
    }
}

BinarySearchTree::BinaryNode *
BinarySearchTree::findMin(BinaryNode *t) const {
        if (t == NULL)  return NULL;
        if (t->left == NULL)    return t;
        return findMin(t->left);
}

BinarySearchTree::BinaryNode *
BinarySearchTree::findMax(BinaryNode *t) const {
    if (t == NULL)  return NULL;
    if (t->right == NULL)    return t;
    return findMin(t->right);
}

bool BinarySearchTree::contains( const int & x, BinaryNode *t) const {
    if (!x)  return false;
    if (x < t->element) return contains(x, t->left);
    if (x > t->element) return contains(x, t->right);
    if (x == t->element) return true;
}

void BinarySearchTree::printTree(BinaryNode *t) const {
    if (!t) return;
    if (t->left)    printTree(t->left);
    cout << t->element << endl;
    if (t->right)   printTree(t->right);
}
