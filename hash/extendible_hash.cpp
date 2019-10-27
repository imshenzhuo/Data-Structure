#include <random>
#include <ctime>
#include <iostream>
#include <list>

#include "extendible_hash.h"

namespace cmudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size)
        :globalDepth(0), bucketMaxSize(size), numBuckets(1) {
  bucketTable.push_back(std::make_shared<Bucket>(0));
}

/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) const {
  return std::hash<K>{}(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const {
  return globalDepth;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
  if (bucket_id >= bucketTable.size())
    return -1;
  if (!bucketTable[bucket_id] || bucketTable[bucket_id]->items.empty()) 
    return -1;
  return bucketTable[bucket_id]->localDepth;
}

/*
 * helper function to return current number of bucket in hash table
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const {
  return numBuckets;
}

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) {
  //https://en.cppreference.com/w/cpp/thread/mutex
  std::lock_guard<std::mutex> guard(mutex);

  auto index = getBucketIndex(key);
  std::shared_ptr<Bucket> bucket = bucketTable[index];
  if (bucket != nullptr && bucket->items.find(key) != bucket->items.end()) {
    value = bucket->items[key];
    return true;
  }
  return false;
}

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) {
  std::lock_guard<std::mutex> guard(mutex);

  auto index = getBucketIndex(key);
  std::shared_ptr<Bucket> bucket = bucketTable[index];

  if (bucket == nullptr || bucket->items.find(key) == bucket->items.end()) {
    return false;
  }
  bucket->items.erase(key);
  return true;
}

template <typename K, typename V>
int ExtendibleHash<K, V>::getBucketIndex(const K &key) const {
  return HashKey(key) & ((1 << globalDepth) - 1);
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) {
  std::lock_guard<std::mutex> guard(mutex);

  auto index = getBucketIndex(key);
  std::shared_ptr<Bucket> targetBucket = bucketTable[index];

  // bucket满了 分裂
  while (targetBucket->items.size() == bucketMaxSize) {
    // directory要翻倍 D+1
    if (targetBucket->localDepth == globalDepth) {
      size_t length = bucketTable.size();
      for (size_t i = 0; i < length; i++) {
        // 此处新增的位数并不是在原有的D位append, 而是在前面
        // 联系后面的mask变量不难理解
        bucketTable.push_back(bucketTable[i]);
      }
      globalDepth++;
    }
    int mask = 1 << targetBucket->localDepth;
    numBuckets++;
    auto zeroBucket = std::make_shared<Bucket>(targetBucket->localDepth + 1);
    auto oneBucket = std::make_shared<Bucket>(targetBucket->localDepth + 1);
    for (auto item : targetBucket->items) {
      size_t hashkey = HashKey(item.first);
      if (hashkey & mask) {
        oneBucket->items.insert(item);
      } else {
        zeroBucket->items.insert(item);
      }
    }
    for (size_t i = 0; i < bucketTable.size(); i++) {
      if (bucketTable[i] == targetBucket) {
        if (i & mask) {
          bucketTable[i] = oneBucket;
        } else {
          bucketTable[i] = zeroBucket;
        }
      }
    }
    index = getBucketIndex(key);
    targetBucket = bucketTable[index];
  } //end while

  targetBucket->items[key] = value;
}

template <typename K, typename V>
V ExtendibleHash<K, V>::get(const K &key) {
  auto bucketId = getBucketIndex(key);
  std::shared_ptr<Bucket> target = bucketTable[bucketId];
  auto ret = target->items.find(key);
  if (ret == target->items.end()) 
    return -1;
  return ret->second;
}

} // namespace cmudb


int main(int argc, char const *argv[])
{
  const int count = 100000;
  int miss = 0;
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1,count); // distribution in range [1, 6]

  int data[count];
  for (int i = 0; i < count; ++i)
  {
    data[i] = dist6(rng);
  }

  cmudb::ExtendibleHash<int, int> ehs(5);

  const clock_t begin_time_insert = clock();
  for (int i = 0; i < count; ++i)
  {
    ehs.Insert(i, i);
  }
  std::cout << "insert time is " << float( clock () - begin_time_insert ) /  CLOCKS_PER_SEC << std::endl;

  const clock_t begin_time_get = clock();
  
  for (int i = 0; i < count; ++i)
  {
    if(ehs.get(i) != i) {
      miss ++;
    }
    /* code */
  }
  std::cout << "get time is " << float( clock () - begin_time_get ) /  CLOCKS_PER_SEC << std::endl;
  std::cout << "\nmiss " << miss << std::endl;

  /* code */
  return 0;
}