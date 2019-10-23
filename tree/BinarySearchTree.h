#include <iostream>
#include <string> 
using std::string;

class TreeNode {
private:
    int value;
    TreeNode *left;
    TreeNode *right;
public:
    TreeNode(int v)
        : value(v), left(nullptr), right(nullptr)
    { }
    TreeNode(const TreeNode& rhs):
        value(rhs.value), left(rhs.left), right(rhs.right) {
    }
    TreeNode& operator=(const TreeNode& rhs);
    ~TreeNode() {
        if (left) {
            delete left;
            left = nullptr;
        }
        if (right) {
            delete right;
            right = nullptr;
        }
    }
    int getValue() {
        return value;
    }
    TreeNode *insert(int v);
    void printTree();
};

class BinStrTree
{
private:
    TreeNode *root;
public:
    BinStrTree(int v): root(new TreeNode(v)) {}
    BinStrTree(const BinStrTree& bst):
        root(new TreeNode(*bst.root)){};
    BinStrTree& operator=(const BinStrTree& bst);
    ~BinStrTree() { delete root;}

    void printTree() { root->printTree(); }
    bool insert(int);
    
};
