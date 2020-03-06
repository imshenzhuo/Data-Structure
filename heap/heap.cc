#include "heap.h"

using namespace std;

void BinaryHeap::insert(const int &x) {
    if(currentSize == array.size() - 1)
        array.resize(array.size() * 2);
    int hole = ++currentSize;
    for(;hole > 1 && x < array[hole / 2]; hole /= 2) 
        array[hole] = array[hole / 2];
    array[hole] = x;
}

// 删除小顶堆的最小值
int BinaryHeap::popMin() {
    if (isEmpty())  return -1;
    int res = array[1];
    // 不能破坏完全二叉树的结构
    // 用最后一个元素覆盖堆顶元素 => array[1] = array[currentSize];
    // 并且删除最后一个元素 => currentSize--;
    array[1] = array[currentSize--];
    // 下沉
    percolateDown(1);
    return res;
}

// 删除指定位置的元素
int BinaryHeap::popIndex(int position) {
    if (position > currentSize)  return -1;
    int res = array[position];
    // 不能破坏完全二叉树的结构
    array[position] = array[currentSize--];
    // 下沉
    percolateDown(position);
    return res;
}



void BinaryHeap::percolateDown(int hole) {
    int child;
    // 暂存要删除的元素
    int t = array[hole];
    //只要有子树, 继续循环
    for(; hole * 2 <= currentSize; hole = child) {
        // 左子树
        child = hole * 2;
        // child最终代表最小的子树index
        if (child != currentSize && array[child + 1] < array[child])
            child++;
        
        // 需要下沉
        if (array[child] < t) 
            array[hole] = array[child];
        else
            break;
    }
    // 最后替换
    array[hole] = t;
}

void BinaryHeap::topK(int val) {
    array[1] = val;
    percolateDown(1);
}

