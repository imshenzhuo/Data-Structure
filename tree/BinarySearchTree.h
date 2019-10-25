/**
 * c++实现二叉搜索树 
 * 
 * 
 */ 

#include <iostream>
#include <string> 
using namespace std;

template <typename Comparable>
class BinarySearchTree
{
public:
    BinarySearchTree() {}
    BinarySearchTree(const BinarySearchTree& bst);
    const BinarySearchTree& operator=(const BinarySearchTree& bst);
    ~BinarySearchTree(){
        if(root) {
            cout << "delete tree "  << endl;
            delete root;
            root = nullptr;
        }
    };

    const Comparable & findMin() const {
        findMin(root);
    }
    const Comparable & findMax() const{
        findMax(root);
    }
    bool contains( const Comparable & x) const {
        return contains(x, root);
    }
    bool isEmpty() const;
    void printTree() const {
        printTree(root);
    }

    void makeEmpty();
    void insert( const Comparable & x) {
        insert(x, root);
    }
    void remove( const Comparable & x) {
        remove(x, root);
    }


private:
    
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;
        int N;

        BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt)
            : element(theElement), left(lt), right(rt) {  }   
        ~BinaryNode() {
            cout << "delete node " << this->element << endl;
            if(left) {
                delete left;
                left = nullptr;
            }
            if (right) {
                delete right;
                right = nullptr;
            }
        }
    };
    
    BinaryNode *root;
    
    void insert(const Comparable & x, BinaryNode * &t) const {
        if (!t) t = new BinaryNode(x, NULL, NULL);
        else if( x < t->element)    insert(x, t->left);
        else if(t->element < x)     insert(x, t->right);
    }
    void remove(const Comparable & x, BinaryNode * &t) const {
        if (!t) return;
        if (x < t->element) remove(x, t->left);
        else if(x > t->element) remove(t, t->right);
        else if(!(t->left)&&!(t->right)) {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }else {
            BinaryNode *oldNode = t;
            t = (t->left!=NULL) ? t->left : t->right;
            delete oldNode;
        }
    }
    BinaryNode * findMin(BinaryNode *t) const {
        if (t == NULL)  return NULL;
        if (t->left == NULL)    return t;
        return findMin(t->left);
    }
    BinaryNode * findMax(BinaryNode *t) const {
        if (t == NULL)  return NULL;
        if (t->right == NULL)    return t;
        return findMin(t->right);
    }
    bool contains( const Comparable & x, BinaryNode *t) const {
        if (x == NULL)  return false;
        if (x < t->element) return contains(x, t->left);
        if (x > t->element) return contains(x, t->right);
        if (x == t->element) return true;
    }
    void makeEmpty(BinaryNode *&t);
    void printTree(BinaryNode *t) const {
        if (!t) return;
        if (t->left)    printTree(t->left);
        cout << t->element << endl;
        if (t->right)   printTree(t->right);
    }
    BinaryNode *clone(BinaryNode *t) const;
};


// diff null nullptr