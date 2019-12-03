# 跳跃表的C++实现

> 参考leveldb

## 接口

### 公有接口

``` c++
template <typename Key, class Comparator>
class SkipList {
public:
// 所有的public接口
explicit SkipList(Comparator cmp, Arena* arena);

SkipList(const SkipList&) = delete;
SkipList& operator=(const SkipList&) = delete;

// Insert key into the list.
// REQUIRES: nothing that compares equal to key is currently in the list.
void Insert(const Key& key);

// Returns true iff an entry that compares equal to key is in the list.
bool Contains(const Key& key) const;

// Iteration over the contents of a skip list
class Iterator {...};
};
```

跳跃表的操作无非就是读写, 所以公有的接口也很简单, 值得注意的地方是

- 读的操作封装到一个内部类中
- 将key的比较器和内存分配器抽象

### 私有数据成员

``` c++
struct Node; // 声明内部数据结构
enum { kMaxHeight = 12 };
// Immutable after construction
Comparator const compare_;
Arena* const arena_;  // Arena used for allocations of nodes

Node* const head_;

// Modified only by Insert().  Read racily by readers, but stale
// values are ok.
std::atomic<int> max_height_;  // Height of the entire list

// Read/written only by Insert().
Random rnd_;
```

核心的数据成员就是`head_`指向数据的节点, 其余还有分配器 比较器 当前高度 最大高度以及插入数据时所需的随机数产生器.

- 私有数据成员的命名规则 `xxx_`
- 使用枚举???代替#define
- 该常量的就用常量

### 说明

leveldb实现的跳跃表并没有像模型图中那样, 有很多个Node, 最下面是Data Node, 上面都是Index Node; 而是利用了每一列的Key相同的特点, 给每一个Node都最多设置`kMaxHeight`个指针(因为分配器没有必要分配的没有分配), 这样既可以节省重复的key空间也可以直接省去上一层指向下一层的指针. 

此处应有图



## 实现

1. 找到要插入的地方

``` c++
template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* // 返回值类型
SkipList<Key, Comparator>::FindGreaterOrEqual(const Key& key,
                                              Node** prev) const {
  // prev是指针数组
  Node* x = head_;
  int level = GetMaxHeight() - 1; // 从top到down
  while (true) {
    // 直接比较下一个 既可以避免对dumb头的访问, 又可以达到比较下一个的效果
    Node* next = x->Next(level); 
    if (KeyIsAfterNode(key, next)) {
      // Keep searching in this list
      x = next;
    } else {
      if (prev != nullptr) prev[level] = x;
      if (level == 0) {
        return next;
      } else {
        // Switch to next list
        level--;
      }
    }
  }
}
```

执行结束后返回数据插入位置的前一位, 将路径存到了prev数组中

2. 随机向上

``` c++
template <typename Key, class Comparator>
int SkipList<Key, Comparator>::RandomHeight() {
  // Increase height with probability 1 in kBranching
  static const unsigned int kBranching = 4;
  int height = 1;
  // 25%概率
  while (height < kMaxHeight && ((rnd_.Next() % kBranching) == 0)) {
    height++;
  }
  assert(height > 0);
  assert(height <= kMaxHeight);
  return height;
}
```

模拟, 每次25%概率向上, 达到继续否则返回当前高度

3. 插入数据

``` c++
template <typename Key, class Comparator>
void SkipList<Key, Comparator>::Insert(const Key& key) {
  // TODO(opt): We can use a barrier-free variant of FindGreaterOrEqual()
  // here since Insert() is externally synchronized.
  // 找到位置 并且记录路径 prev (每一项都是该层的前一个Node)
  Node* prev[kMaxHeight];
  Node* x = FindGreaterOrEqual(key, prev);

  // Our data structure does not allow duplicate insertion
  assert(x == nullptr || !Equal(key, x->key));

  int height = RandomHeight(); // 向上的高度
  if (height > GetMaxHeight()) { // 如果超过了当前高度, 每一个超出的节点前节点都是head_指针
    for (int i = GetMaxHeight(); i < height; i++) {
      prev[i] = head_;
    }
    // It is ok to mutate max_height_ without any synchronization
    // with concurrent readers.  A concurrent reader that observes
    // the new value of max_height_ will see either the old value of
    // new level pointers from head_ (nullptr), or a new value set in
    // the loop below.  In the former case the reader will
    // immediately drop to the next level since nullptr sorts after all
    // keys.  In the latter case the reader will use the new node.
    max_height_.store(height, std::memory_order_relaxed); // 更新当前最大高度
  }

  x = NewNode(key, height);
  // 逐层更新链表
  for (int i = 0; i < height; i++) {
    // NoBarrier_SetNext() suffices since we will add a barrier when
    // we publish a pointer to "x" in prev[i].
    x->NoBarrier_SetNext(i, prev[i]->NoBarrier_Next(i));
    prev[i]->SetNext(i, x);
  }
}
```



## 分配器

``` c++
template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::NewNode(
    const Key& key, int height) {
  char* const node_memory = arena_->AllocateAligned(
      sizeof(Node) + sizeof(std::atomic<Node*>) * (height - 1));
  return new (node_memory) Node(key);
}
```

每次分配的内存至少一页, 减少内存分配调用次数以及内存碎片

## 并发

写操作需要外部同步，很可能是互斥锁。读取需要保证在读取过程中不会破坏SkipList。除此之外，读取进度无需任何内部锁定或同步。

不变量

- 分配的节点只有在跳表被销毁时候才被删除
- 将节点链接到SkipList之后，除next / prev指针外，Node的内容是不变的。

``` c++
x->NoBarrier_SetNext(i, prev[i]->NoBarrier_Next(i)); // 不影响不变性  但是要原子性
prev[i]->SetNext(i, x); // 重排序
```



- memory_order_acquire
- memory_order_release
- memory_order_relaxed

## 语法

函数模板中的typename和class有什么区别

为什么模板类的实现不能分开写

模板类中的内部类的实现体

- public内部类实现前要声明 inline
- private内部类函数实现前要声明 typename