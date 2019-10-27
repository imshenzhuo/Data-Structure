/*
 * extendible_hash.h : implementation of in-memory hash table using extendible
 * hashing
 *
 * Functionality: The buffer pool manager must maintain a page table to be able
 * to quickly map a PageId to its corresponding memory location; or alternately
 * report that the PageId does not match any currently-buffered page.
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "hash_table.h"

namespace cmudb {

template <typename K, typename V>
class ExtendibleHash : public HashTable<K, V> {
  
  struct Bucket {
    explicit Bucket(int depth):localDepth(depth) {};
    int localDepth;
    // 不是简单的线性结构, 而是使用了c++标准库
    std::map<K, V> items;
  };

public:
  // constructor
  ExtendibleHash(size_t size);
  // helper function to generate hash addressing
  size_t HashKey(const K &key) const;
  // helper function to get global & local depth
  int GetGlobalDepth() const;
  int GetLocalDepth(int bucket_id) const;
  int GetNumBuckets() const;
  // lookup and modifier
  bool Find(const K &key, V &value) override;
  bool Remove(const K &key) override;
  void Insert(const K &key, const V &value) override;
  V get(const K &key);

private:
  // add your own member variables here
  int getBucketIndex(const K &key) const;

  int globalDepth;
  // 桶的大小也就是M, 一般是磁盘块存储数据的个数
  size_t bucketMaxSize;
  // 当前桶的数量
  int numBuckets;
  
  std::vector<std::shared_ptr<Bucket>> bucketTable;
  std::mutex mutex;
};
} // namespace cmudb