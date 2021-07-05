//简单实现的hash表 通过不同的hash函数实现存储不同的内容 
#ifndef __HASH_TABLE__H
#define __HASH_TABLE__H
#ifdef __cplusplus
extern "C"{
#endif
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<errno.h>
#include<assert.h>
#include<stdio.h>
#include "simple_str.h"
//hash函数

typedef struct table_entry{
    void*                  key_;
    void*                  val_;
    struct table_entry*    next_;
} table_entry;
typedef uint32_t(*hash_function)(const void*);
typedef int(*cmp)(const void*, const void* );
typedef void*(*dump)(const void *);//拷贝函数  拷贝key 和value
typedef void(*destruct)(void *);//析构函数

typedef struct HashTypeFunction{
    hash_function    hash_;//获取hash值
    cmp              comp_;//key值比较
    dump             key_dump_;//拷贝key
    dump             value_dump_;//拷贝value
    destruct         free_key_;//key 析构函数
    destruct         free_val_;//val 析构函数
}HashTypeFunction;


typedef struct hash_table{
    table_entry**       table_;
    uint32_t            size_;//总容量 //保证是8的倍数 
    uint32_t            use_;//包含元素
    HashTypeFunction*   hash_functions_;
}hash_table;

typedef struct hash_table_iterator{
    table_entry*        entry_;//当前节点
    hash_table*         table_;//待遍历的hash表
    int                 index_;
}hash_table_iterator;

//占比超过 75%就扩容
#define   ResizeLimit 0.75
#define   HashSlot(t, key)  t->hash_functions_->hash_(key)&(t->size_-1)
#define   HashError(msg) do{\
    int rt=errno;\
    fprintf( stderr, "HashTable error %s  %s\n", msg, strerror(rt));\
}while(0);

//针对 指针不用拷贝的情况
#define HashSetKey(t, entry, key ) \
    if(t->hash_functions_->key_dump_)\
        (entry)->key_ = t->hash_functions_->key_dump_(key);\
    else\
        (entry)->key_ = key 

#define HashSetValue(t,entry,val)\
    if(t->hash_functions_->value_dump_)\
        (entry)->val_ = t->hash_functions_->value_dump_(val);\
    else\
        (entry)->val_ = val;

#define HashFreeKey(t,entry) \
    if(t->hash_functions_->free_key_)\
        t->hash_functions_->free_key_(entry->key_)

#define HashFreeVal(t,entry)\
    if(t->hash_functions_->free_val_)\
        t->hash_functions_->free_val_(entry->val_)

//构造函数
hash_table* hash_table_construct( HashTypeFunction* );

//查找
table_entry* hash_table_find( hash_table* table, void*key );

//插入
void hash_table_insert( hash_table*table, void*key, void*val );

//删除
void hash_table_del(hash_table*table, void*key );
//清理
void hash_table_clear(hash_table*table);

//hash扩容
void hash_table_growup(hash_table*table, size_t size );
// 迭代器
hash_table_iterator *
hash_table_get_iterator(hash_table* table );

table_entry* 
hash_table_next(hash_table_iterator * it );

void
hash_table_release_iterator( hash_table_iterator*it);

//一些常用的hash 函数
//key取socket描述符的hash函数
uint32_t  SocketFdHash(const void*key );
//key为int生成hash值
uint32_t IntergerHash( const void*key );
//int比较函数
int IntergerCompare(const void*rv, const void*lv);
//拷贝int key
void* IntergerDump(const void* );

//key为string的hash函数
uint32_t StringHash( const void*key );
//string 比较函数
int StringCompare(const void*rv,const void*lv);
//string 无视大小写比较
int StringCaseCompare(const void*rv,const void*lv );
//值类型为sds的析构函数
void SDSFree(void*);
//拷贝sds
void* SdsDump(const void* );

//普通字符拷贝
void* StringDump(const void* );


//key为整形 value 为sds
HashTypeFunction IntergeHashFunctions={
   IntergerHash,
   IntergerCompare,
   IntergerDump,
   SdsDump,
   free,
   SDSFree
};
//key为socket values 为指针
HashTypeFunction SocketHashFunctions={
    SocketFdHash,
    IntergerCompare,
    IntergerDump,
    NULL,
    free,
    NULL
};
//key 为sds value也是sds的类型
HashTypeFunction SdsHashFunctions={
    StringHash,
    StringCompare,
    SdsDump,
    SdsDump,
    SDSFree,
    SDSFree
};
//key为普通字符串 valus也是普通字符串
HashTypeFunction StringHashFunctions={
    StringHash,
    StringCompare,
    StringDump,
    StringDump,
    free,
    free
};




#ifdef HASH_TABLE_TEST
#include "tool.h"
void hash_test(unsigned count );
#endif







#ifdef __cplusplus
}
#endif
#endif
 
