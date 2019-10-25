#include <iostream>
#include <random>
#include "BinarySearchTree.h"

int main(int argc, char const *argv[])
{

    const int N = 5;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,600); // distribution in range [1, 6]

    BinarySearchTree<int> *bst = new BinarySearchTree<int>;
    /* code */
    for (size_t i = 0; i < N; i++)
    {
        bst->insert(dist6(rng));
    }
    bst->printTree();
    delete bst;
    bst = nullptr;
    return 0;
}
