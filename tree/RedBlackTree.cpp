#include <cstdio>
#include <iostream>
#include "RedBlackTree.h"

int main(int argc, char const *argv[])
{
    RedBlackTree<int, int> rbt(0, 3);
    for(int i = 1; i<12; ++i) 
        rbt.insert(i, i);

    printf("depth is %d\n", rbt.getDepth());
    rbt.printTree();
    printf("printByLevel-----------\n");
    for(int i=0; i<=rbt.getDepth(); ++i)
        rbt.printByLevel(i);
    printf("printByLevelWithRB------\n");
    for(int i=0; i<=rbt.getDepth(); ++i)
        rbt.printByLevelWithRB(i);
    return 0;
}
