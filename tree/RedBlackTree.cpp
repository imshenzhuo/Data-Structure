#include <cstdio>
#include "RedBlackTree.h"

int main(int argc, char const *argv[])
{
    RedBlackTree<int, int> *rbt = new RedBlackTree<int, int>;
    rbt->insert(3, 0);
    rbt->insert(1, 0);
    rbt->insert(2, 0);
    rbt->insert(5, 0);
    rbt->insert(4, 0);
    rbt->printTree();
    delete rbt;

    RedBlackTree<int, int> rbt1;
    rbt1.insert(3, 0);
    rbt1.insert(1, 0);
    rbt1.insert(2, 0);
    rbt1.insert(5, 0);
    rbt1.insert(4, 0);
    rbt1.printTree();

    RedBlackTree<int, int> rbt2;
    for(int i = 1; i<1; ++i) 
        rbt2.insert(i, i);

    printf("depth is %d\n", rbt2.getDepth());
    rbt2.printTree();
    return 0;
}
