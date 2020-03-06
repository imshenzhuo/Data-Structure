#include <iostream>
#include "heap.h"

using namespace std;

int main(int argc, char const *argv[])
{
    // 使用小顶堆实现top3 
    BinaryHeap h;
    vector<int> ivec{1,4,2,6,8,4,5,2};
    int i = 0;
    for(; i < 3; i++) {
        h.insert(ivec[i]);
    }
    for(; i < ivec.size(); i++) {
        if (ivec[i] > h.findMin())  {
            // --------------method1------------------
            // 这两步是有重复的 
            // popMin 会让最后一个元素替换堆顶元素, 然后下沉
            // insert的话会在最后添加一个元素 上浮
            // 应该只用一步 直接替换和下沉
            // h.popMin();
            // h.insert(ivec[i]);
            // --------------method2------------------
            h.topK(ivec[i]);
        }
    }
    while (!h.isEmpty()) {
        cout << h.popMin() << endl;
    } 
    return 0;
}
