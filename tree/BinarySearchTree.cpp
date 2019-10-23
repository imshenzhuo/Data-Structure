#include <iostream>
#include <random>
#include "BinarySearchTree.h"

TreeNode& TreeNode::operator=(const TreeNode& rhs)
{
    if (left) {
        delete left;
        left = nullptr;
    }
    if (right) {
        delete right;
        right = nullptr;
    }
    value = rhs.value;
    left = rhs.left;
    right = rhs.right;
    return *this;
}
BinStrTree& BinStrTree::operator=(const BinStrTree& bst)
{
    TreeNode* new_root = new TreeNode(*bst.root);
    delete root;
    root = new_root;
    return *this;
}

bool BinStrTree::insert(int v) {
    if (!root)  return false;
    return root->insert(v);
}

TreeNode *TreeNode::insert(int v) {

    if (v == value) {
        return nullptr;
    }
    if (v > value)  {
        if(!right)  right = new TreeNode(v);
        else right = right->insert(v);
    } else {
        if(!left)  left = new TreeNode(v);
        else left = left->insert(v);
    }
    return this;
}

void TreeNode::printTree() {
    if (left)   left->printTree();
    std::cout << value << "   " << std::endl;
    if (right)  right->printTree();
}

int main(int argc, char const *argv[])
{
    const int N = 500000;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,600); // distribution in range [1, 6]

    std::cout << dist6(rng) << std::endl;
    int a = 4;
    BinStrTree bst(4);
    /* code */
    for (size_t i = 0; i < N; i++)
    {
        bst.insert(dist6(rng));
    }
    bst.printTree();
    return 0;
}
