/**
 * c++实现二叉搜索树 
 * 
 * 
 */ 

#include <iostream>
#include <string> 
using namespace std;

class BinarySearchTree
{
public:
    BinarySearchTree() {}
    BinarySearchTree(const BinarySearchTree& bst);
    const BinarySearchTree& operator=(const BinarySearchTree& bst);
    ~BinarySearchTree();

    const int & findMin() const;
    const int & findMax() const;
    bool contains( const int & x) const ;
    bool isEmpty() const;
    int getSize() const;
    void printTree() const ;

    void insert( const int & x);
    void remove( const int & x);


private:
    
    struct BinaryNode
    {
        int element;
        BinaryNode *left;
        BinaryNode *right;
        int N;

        BinaryNode(const int & theElement, BinaryNode *lt, BinaryNode *rt)
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
    
    void insert(const int & x, BinaryNode * &t) const;
    void remove(const int & x, BinaryNode * &t) const;
    BinaryNode * findMin(BinaryNode *t) const ;
    BinaryNode * findMax(BinaryNode *t) const ;
    bool contains( const int & x, BinaryNode *t) const ;
    void printTree(BinaryNode *t) const ;
};