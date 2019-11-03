#include <cstdio>
#include <cassert>

template <typename Key, typename Value>
class RedBlackTree {
    public:
        RedBlackTree() {
            cache = new Node(0, 0, 1, false);
        }
        RedBlackTree(Key k, Value v) {
            root = new Node(k, v, 1, false);
            cache = new Node(k, v, 1, false);
        }
        ~RedBlackTree() {  delete root;   }


        bool contains(Key k) {  return contains(root, k);  }
        const Value &findMin() const {  return findMin(root); }
        const Value &findMax() const {  return findMax(root); }
        Value get(Key k) {
            if (cache->key == k)    return cache->value;
            return get(root, k);
        }

        void insert(Key k, Value v) {
            root = insert(root, k, v);
            root->color = BLACK;
        }

        int getDepth() {
            int Depth = 0;
            Node *t = root;
            while(t->right) {
                t = t->right;
                Depth++;
            }
            return Depth;
        }

        void printTree() {  return printTree(root); }

        /**
         * 二叉树层次打印
         * @para n 层树  0 代表root
         */ 
        void printByLevel(int n) { printByLevel(root, n); }

        /**
         * 带红黑树层次打印
         * @para n 层树  0 代表root
         */
        void printByLevelWithRB(int n) { printByLevelWithRB(root, n);   }       


        // 统计有多少红节点 多少黑节点
        void count(int &r, int &b) {
            NumberOfRed = 0;
            NumberOfBlack = 0;
            count(root);
            r = NumberOfRed;
            b = NumberOfBlack;
        }

        int size() { return size(root); }

    private:
        struct Node
        {
            Key key;
            Value value;
            Node *left, *right;
            int N;
            bool color;

            Node(Key k, Value val, int N,  bool color) 
            : key(k), value(val), N(N), color(color) {
                left = right = nullptr;
            }
            ~Node() {
                if(left) {
                    delete left;
                    left = nullptr;
                }
                if (right) {
                    delete right;
                    right = nullptr;
                }
            }
        };

        static const bool RED = true;
        static const bool BLACK = false;
        Node *root;
        Node *cache;
        int NumberOfRed;
        int NumberOfBlack;

        /*********************insert****************************/
        // 节点和父节点之间的连接颜色
        bool isRed(Node *x) const {
            if (!x) return false;
            return x->color;
        }
        
        void filpColors(Node *h) {
            h->color = RED;
            h->left->color = BLACK;
            h->right->color = BLACK;
        }

        /** core code
         * 旋转的目的是改变红链接的指向
         * 所以左旋转的话 h的右链接是红链接
         */  
        Node *rotateLeft(Node *h) {
            assert(h->right->color == RED);
            Node *x = h->right;
            h->right = x->left;
            x->left = h;
            // h的color既可能是red 也可能是black
            x->color = h->color; 
            h->color = RED;
            return x;
        }
        /** core code
         * 旋转的目的是改变红链接的指向
         * 所以右旋转的话 h的左链接是红链接
         */  
        Node *rotateRight(Node *h) {
            Node *x = h->left;
            h->left = x->right;
            x->right = h;
            // h的color既可能是red 也可能是black
            x->color = h->color;
            h->color = RED;
            return x;
        }

        Node* insert(Node *cur, Key k, Value v) {
            if (!cur) return new Node(k, v, 1, RED);
            if (k < cur->key) cur->left = insert(cur->left, k, v);
            else if (k > cur->key)    cur->right = insert(cur->right, k, v);
            else cur->value = v;

            if (isRed(cur->right) && !isRed(cur->left))   cur = rotateLeft(cur);
            if (isRed(cur->left) && isRed(cur->left->left)) cur = rotateRight(cur);
            if (isRed(cur->left) && isRed(cur->right))  filpColors(cur);

            cur->N = size(cur->left) + size(cur->right) + 1;
            return cur;
        }

        /*********************get****************************/

        const Value &findMin(Node *t) const {
            while (t->left)
                t = t->left;
            return t->value;
        }
        const Value &findMax(Node *t) const {
            while (t->right)
                t = t->right;
            return t->value;
        }

        Value get(Node *n, Key k) {
            if (!n) return -1;
            if (k > n->key) return get(n->right, k);
            else if (k < n->key) return get(n->left, k);
            else { 
                cache->key = k;
                cache->value = n->value;
                return n->value;
            }
        }

        /*********************print****************************/
        // 中序打印
        void printTree(Node *n) {
            if (!n) return;
            if (n->left)    printTree(n->left);
            // printf("%d %d\n", n->key, n->value);
            printf("%d \n", n->key);
            if (n->right)   printTree(n->right);
        }

        // 二叉树层次打印
        void printByLevel(Node *t ,int n) {
            if (n==0) printf("%d\t", t->key);
            else {
                if (t->left)    printByLevel(t->left, n - 1);
                if (t->right)  printByLevel(t->right, n - 1);
            }
        }
        // 带红黑树的层次打印
        void printByLevelWithRB(Node *t ,int n) {
            if (n==0 && t->left && t->left->color)
                printf("%d--", t->left->key);
            if (n==0) printf("%d\t", t->key);
            else {
                if (t->left && t->left->color) {
                    if (t->left->left)    
                        printByLevelWithRB(t->left->left, n-1);
                    if (t->left->right)    
                        printByLevelWithRB(t->left->right, n-1);
                } else {
                    if (t->left)  printByLevelWithRB(t->left, n-1);    
                }
                if (t->right)  printByLevelWithRB(t->right, n-1);
            }
        }

        // /*********************delete****************************/
        // Node* moveRedLeft(Node *h) {
        //     filpColors(h);
        //     if (isRed(h->right->left)) {
        //         h->right = rotateRight(h->right);
        //         h = rotateLeft(h);
        //     }
        //     return h;
        // }
        // Node* deleteMin(Node *h) {
        //     if (!h->left) {
        //         delete h->left;
        //         h->left = nullptr;
        //         return nullptr;
        //     }

        // }

        bool contains(Node *t, Key k) {
            if (!t) return false;
            if (k > t->key) return contains(t->right, k);
            if (k < t->key) return contains(t->left, k);
            return true;
        }

        void count(Node *h) {
            if (!h) return;
            if (h->color)   ++NumberOfRed;
            else ++NumberOfBlack;
            if (h->left)    count(h->left);
            if (h->right)   count(h->right);
        }

        int size(Node *h) {
            if (!h) return 0;
            return h->N;
        }
};