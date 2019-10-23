使用cpp实现不同种类的hash function 和 hash schema, 并且对性能做出比较

- chain
- Extendible Hash Table

## extendible hash table

由两部分组成: 目录 + bucket

插入数据

1. 如果索引到的bucket没有满, 直接插入
2. 如果索引到的bucket满员, split
   1. 如果该bucket的位数等于全局位数
      1. 全局位数加1, 目录翻倍
   2. 将要split的bucket和新添加的entry重新按照新的索引放入bucket

