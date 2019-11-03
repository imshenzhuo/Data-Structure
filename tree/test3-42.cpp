#include "RedBlackTree.h"
#include <cstdio>
#include <random>

static const int N[] = {10000, 100000, 1000000};

int main(int argc, char const *argv[])
{
    std::random_device dev;
    std::mt19937 rng(dev());
    
    for (size_t i = 0; i < 3; i++)
    {
        // distribution in range [1, N[i]]
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1,N[i]); 
        RedBlackTree<int, int> rbt(0, 0);
        // insert 
        int key,red, black;
        for (int k = 0; k < 100; k++) {
            for (size_t j = 0; j < N[i]; j++) {
                key = dist6(rng);
                rbt.insert(key, key);
            }
            rbt.count(red, black);
            printf("red = %d black = %d ", red, black);
            printf("sum = %d  ", red + black);
            printf("precent = %f\n", 1.0 * red / (red + black));
            printf("depth = %d\n", rbt.getDepth());
        }
        
    }
    
    return 0;
}
