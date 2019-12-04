/*
 * LRU(latest recently use)热点数据缓存
 * 在一些有用到redis的场景中，一些热点数据会经常访问，
 * 如果全部都从redis里获取明显不合适，缓存在内存中是个很好的方法
 * 基于c++11
 * */
#ifndef __LRU_CACHE
#define __LRU_CACHE
#include "def.h"
#include<list>
#include<unordered_map>
#include<utility>

template<typename KeyType,typename ValueType>
class LRUCache{
    public:
        using ValIteratorType = typename std::list<KeyType>::iterator;
        using MapKeyVal =  std::unordered_map<KeyType,std::pair<ValueType,ValIteratorType> >;
        explicit LRUCache( size_t maxCount  ):m_capacity(maxCount)
        {
            Clear();
            m_map.reserve(m_capacity);
        }

        void Add(const KeyType& key,const ValueType& val )
        {
            if( unlikely( Contain(key) ) )                    
            {                                                          
                /*这里我觉得可以更新一下*/
                std::pair<ValueType,ValIteratorType>& it = m_map[key];
                m_list.erase( it.second );
                m_list.push_front(key);
                it.first = val;
                it.second = m_list.begin();
				return ;
            }

            if( unlikely (m_map.size()< m_capacity  ) )
            {
               Insert(key,val);
               return;
            }

            ValIteratorType it = --m_list.end();
            m_map.erase(*it);
            m_list.erase(it);
            Insert(key,val);
            /* 向list添加元素后： 
             * 指向list的迭代器仍有效。即向list中添加元素不影响迭代器失效。
             * 从list删除元素后： 
             * 当我们从list删除一个元素后，指向该元素的迭代器会失效，其他位置不受影响。
             * */
        }

        bool GetVal( const KeyType& key, ValueType& val  )
        {
            if( !Contain( key ) )
                return false;
            std::pair<ValueType,ValIteratorType>& _val = m_map[key];
            if( likely( _val.second != m_list.begin()  ) )
            {
                m_list.push_front( key );
                m_list.erase( _val.second );
                _val.second = m_list.begin();
            }
            val = _val.first;
            return true;
        }

        bool Contain(const KeyType& key )const
        {
            return m_map.count(key) == 1;
        }

        size_t Size()const
        {
            return m_map.size();
        }
        size_t Capacity()const
        {
            return m_capacity;
        }
        void Clear()
        {
            m_list.clear();
            m_map.clear();
        }
    private:
        void Insert( const KeyType& key, const ValueType& val  )
        {
            m_list.push_front( key );
            m_map[key]=std::make_pair(val, m_list.begin());
        }
        LRUCache( const LRUCache& )=delete;
        LRUCache& operator=(const LRUCache&)=delete;
        size_t              m_capacity;
        std::list<KeyType>  m_list;
        MapKeyVal           m_map;
        

};
#ifdef LRU_TEST
#include<cassert>
#include<iostream>
#include<string>
#include<memory>
#include<vector>
using namespace std;
void LRU_Test()
{
    LRUCache<int, std::string>  test(10);

    for( int i=0;i<30;i++  )
    {
        test.Add( i,std::to_string(i*10) );
        test.Add(1,"abc");
        test.Add(2,"eee");
    }
    //热点数据必然有1 2
    std::string temp; 
    bool brt = test.GetVal( 1,temp  );
    brt &= ( temp == "abc" );
    assert( brt  );

    brt &= test.GetVal( 2,temp  ) && temp == "eee";
    assert( brt );

    assert( test.Size()==10  );
    for( int i=0;i<50;i++  )
    {
        test.Add( i,std::to_string(i*10) );
        test.Add( i&3,std::to_string(i*10) );
    }
    cout<<endl;
    //热点数据 0 1 2 
    assert( test.GetVal(1,temp)== true );
    assert( test.GetVal(2,temp)== true );


    struct testInfo{
        int  id;
        std::string str;
        testInfo( int _id,const std::string& _str ):id(_id),str(_str){}
    };
    LRUCache<int,std::shared_ptr<testInfo>>  bigCache(100000);

    for( int i=0;i<100000;i++  )
    {
        auto ptr = std::make_shared<testInfo>( i,std::to_string(i*10+4) ); 
        bigCache.Add( i,ptr );
    }

    /*随机取1000个*/
    std::vector<int>v;
    for( int i=0;i<1000;i++  )
    {
        int number = Random( 5000,4000 );
        std::shared_ptr<testInfo> tmp;
        assert(  bigCache.GetVal( number,tmp )==true );
        assert( tmp.get() !=nullptr  );
        assert( tmp->id == number  );
        v.push_back(number);
    }
    int number = Random( 5000,4000 );
    std::shared_ptr<testInfo> tmp;
    bigCache.GetVal(number,tmp);
    cout<<number<<"  "<<tmp->id<<" "<<tmp->str<<endl;
    v.push_back(number);

    /*再插入9500数据 刚才那1000数据还是会存在 这1000就是热点数据*/
    for( int i=100000;i<198000;i++  )
    {
        auto ptr = std::make_shared<testInfo>( i,std::to_string(i*10+4) ); 
        bigCache.Add( i,ptr );
    }

    for( int i=0;i<v.size();i++  )
    {
        int number = v.at(i);
        std::shared_ptr<testInfo> tmp;
        assert(  bigCache.GetVal( number,tmp )==true );
        assert( tmp.get() !=nullptr  );
        assert( tmp->id == number  );
    }
    int number1 = v.at(Random( v.size()-1,0 ));
    std::shared_ptr<testInfo> tmp1;
    bigCache.GetVal(number,tmp1);
    cout<<number1<<"  "<<tmp1->id<<" "<<tmp1->str<<endl;



}
#endif
#endif
