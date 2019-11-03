cc = g++ 
cflags = -g -std=c++11 -O0

files = BinarySearchTree RedBlackTree

all: $(files)

RedBlackTree: RedBlackTree.cpp RedBlackTree.h
	$(cc) $(cflags) -o RedBlackTree RedBlackTree.cpp

BinarySearchTree: BinarySearchTree.cpp BinarySearchTreeTest.cpp
	$(cc) $(cflags) -o BinarySearchTree BinarySearchTree.cpp BinarySearchTreeTest.cpp

clean:
	rm -rf *.out *~ *.o BinarySearchTree RedBlackTree

