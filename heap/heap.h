#include <vector>

// 小顶堆
class BinaryHeap{
public:
    explicit BinaryHeap(int capacity = 100){
        currentSize = 0;
        array.reserve(capacity);
    }
    // explicit BinaryHeap(const std::vector<int> &items);

    bool isEmpty() const {return currentSize < 1;}
    int findMin() const{ return array[1]; }

    void insert(const int &x);
    int popMin();
    int popIndex(int);
    void topK(int val);

private:
    int currentSize;
    std::vector<int> array;

    void buildHeap();
    void percolateDown(int hole);
};