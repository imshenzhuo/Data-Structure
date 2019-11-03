#　2-3树
简单的二叉树在最坏的情况下的性能依旧很遭, 需要额外的操作使得树平衡
2-3树就是一种这样的结构, 并且很容易理解

两种节点构成 2-节点 3-节点 (有n个叉)

从只含有一个3-节点的树开始考虑----
    当插入的节点是3节点时, **临时变为4-节点**, 然后在变为3个2节点
        4节点分为两个2节点 -- 中间的节点向上添加 [core]
    类似的, 共有6种情况 分裂节点是root 分裂节点父节点是2-节点 *2 分裂节点父节点是3-节点 * 3
    递归的insert就好了, 只有当root分裂, 树的高度才会++

在10亿个键中查找, 最多只需要访问30多个节点!

但是有很多缺陷
    有两种数据类型
    可能性略多
    可能涉及到频繁地复制

# 红黑二叉查找树

红黑树的代码好简洁
能把复杂的东西搞得简洁明了!这就是编码能力了吧!很强!

> 用标准的二叉查找树和一些额外信息来表示2-3树
> - 红链接均为左链接
> - 没有任何一个节点同时和两个红链接相连
> - 黑色平衡

### 旋转

为了改变红链接的指向, 满足红黑树的要求
- 没有右红链接
- 没有连续的两个左红链接

#### 核心

1. 左旋转
2. 右旋转

总结出调整的三种情况

1. a-b-c-z
2. b-c-z
3. c-z

``` c
Node* insert(Node *cur, Key k, Value v) {
    if (!cur) return new Node(k, v, 1, RED);
    if (k < cur->key) cur->left = insert(cur->left, k, v);
    else if (k > cur->key)    cur->right = insert(cur->right, k, v);
    else cur->value = v;

    if (isRed(cur->right) && !isRed(cur->left))   cur = rotateLeft(cur);
    if (isRed(cur->left) && isRed(cur->left->left)) cur = rotateRight(cur);
    if (isRed(cur->left) && isRed(cur->right))  filpColors(cur);
    return cur;
}
```

## TODO
剩余红黑树的删除

