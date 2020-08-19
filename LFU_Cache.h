/*
 *  淘汰使用频率最少的元素
 *  LRU( Latest Recently Used )
 *  如果频率一样,则删除最旧的那一个
 *  
 *  可以定义一个结构体 里面存放 value,list的迭代器,int 数 代表访问次数
 *  第一个哈希表存储 key->这个结构
 *  第二个哈希表存储 key时访问次数,value是个链表 链表的值是key
 *  这样 查找/淘汰/删除的复杂度都会是1
 *
 * */
#ifndef __LFU_CACHE_H
#define __LFU_CACHE_H
#include<list>
#include<unordered_map>
template<typename Key,typename Value>
class LFU_Cache{
    using  listiterator = typename std::list<Key>::iterator;
    struct ValNode{
        int   m_freq=1;//访问次数  在插入的时候默认是1 
        listiterator m_iterator;//链表的迭代器 用于快速删除 次数->链表的哈希表中的元素
        Value m_val;//真正的value
    };
    size_t     m_capacity;//容量
    int        m_minFreq;//最小频率
    std::unordered_map<Key,ValNode>  m_contains;
    std::unordered_map<int,std::list<Key>> m_freq2Key;
    public:
    LFU_Cache(size_t  maxsize ):m_capacity(maxsize)
    {
        m_contains.reserve(maxsize);
        m_freq2Key.reserve(maxsize);
    }
    //查找
    bool get(const Key& key, Value& val )
    {
        if( m_contains.count(key)==0 )
            return false;
        increaseFreq(key);
        val = m_contains[key].m_val;
        return true;
    }
    //存入
    void put(const Key&key,const Value& val )
    {
        //已经存在 则更新 并更新次数
        if( m_contains.count(key) )
        {
            m_contains[key].m_val = val;
            increaseFreq(key);
            return;
        }
        //如果已经满了 淘汰频率最少 且最旧的数据 
        if(m_contains.size()>=m_capacity )
            removeMinFre();
        //存数据
        m_minFreq = 1;//新存入的数据 肯定是少访问的
        m_freq2Key[1].push_back(key);

        ValNode& node = m_contains[key];
        node.m_val = val;
        auto end = m_freq2Key[1].end();
        node.m_iterator = --end; 
        node.m_freq = 1;
        return;
    }
    private:
    //增加访问频率
    void increaseFreq(const Key&key )
    {
        int freq = m_contains[key].m_freq;
        m_freq2Key[freq].erase(m_contains[key].m_iterator );//删除在次数->链表 哈希表中的元素

        if( m_freq2Key[freq].empty() && freq == m_minFreq )//删除该元素后 没有这个次数的其他元素了 且这个次数是最小的次数 则更新
            ++m_minFreq;

        m_freq2Key[freq+1].push_back(key);//新次数上的链表存入这个元素
        //更新频率
        ++m_contains[key].m_freq;
        //更新迭代器
        auto end = m_freq2Key[freq+1].end();
        m_contains[key].m_iterator = --end;
    }
    //淘汰访问频率最小 且最旧的数据
    void removeMinFre()
    {
        const Key& key = *m_freq2Key[m_minFreq].begin();//链表头就是访问最小的 且最旧的的数据
        m_contains.erase(key);
        m_freq2Key[m_minFreq].erase(m_freq2Key[m_minFreq].begin());
        //这里即使 m_freq2Key key为m_minFreq的key对应的链表为空了 m_minFreq也不用更新,因为插入的时候赋值为1了
    }
};
#include<cassert>
#include<iostream>
#include<string>
using namespace std;
void LFU_test()
{
    LFU_Cache<int,int> a(2);
    int t=0;
    a.put(1,2);
    a.put(2,3);
    assert(a.get(2,t) && t==3);
    a.put(3,4);
    assert(!a.get(1,t));
    a.get(3,t);
    assert(t == 4 );

    LFU_Cache<int,std::string> b(1000);
    b.put(0,"0" );
    std::string str;
    for( int i=1;i<1000;i++ )
    {
        b.put(i,std::to_string(i) );
        b.get(i,str);
    }
    b.put(1000,"1000" );
    bool ret = b.get(0,str );
    std::cout<<ret<<std::endl;//false

    ret = b.get(500,str );
    std::cout<<ret<<" "<<str<<std::endl;

}
#endif
