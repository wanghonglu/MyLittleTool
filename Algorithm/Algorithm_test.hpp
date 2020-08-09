#ifndef __ALGORITHM_HPP
#define __ALGORITHM_HPP
#include<string>
#include<functional>
#include<memory>
#include<map>
#include<iostream>
#include<cstdint>
#include<algorithm>
#include<limits>
#include <initializer_list>
#include<assert.h>
#include<vector>
#define PrintArray(a) do{\
    std::for_each(a.begin(),a.end(),[](decltype(a[0]) t ){std::cout<<t<<" ";});\
    std::cout<<std::endl;\
}while(0)

class AgorithmSolution{
    public:
        virtual void  Solution()
        {
            std::cout<<m_algorithmName<<std::endl;
        }
        std::string   m_algorithmName="未知算法";

};
class TestSolutionFactory{
public:
    static TestSolutionFactory& Instant()
    {
        static TestSolutionFactory factory;
        return factory;
    }
    void Regiest(int key,AgorithmSolution* t)
    {
        m_factory[key]=t;
    }
    AgorithmSolution* GetOne(int key )
    {
        if(m_factory.count(key))
            return m_factory[key];
        throw std::runtime_error("no Test Solution "+std::to_string(key));
    }
    void PrintAll()
    {
        std::cout<<"usage: "<<std::endl;
        for( const auto& one:m_factory )
        {
            std::cout<<one.first<<"   "<<one.second->m_algorithmName<<std::endl;
        }
    }

private:
    TestSolutionFactory()
    {}
    ~TestSolutionFactory()
    {}
    TestSolutionFactory(const TestSolutionFactory&)=delete;
    TestSolutionFactory& operator =(const TestSolutionFactory& )=delete;
    std::map<int,AgorithmSolution*>    m_factory;
};
//全局范围内不能进行函数调用，所以必须把注册放到构造函数里面去
#define RegistAlgorithm(classname,type)   \
class RegistCalss##type{                  \
    public:                               \
    RegistCalss##type()                         \
    {                                     \
        TestSolutionFactory::Instant().Regiest(type,&m_alg);\
    }                                     \
    private:                              \
    classname     m_alg;                  \
};\
static RegistCalss##type s_RegistCalss##type;

#define GetOneTest(type)  TestSolutionFactory::Instant().GetOne(type)
#define GetHelp()  TestSolutionFactory::Instant().PrintAll()
/*
    链表相关算法实现，这里定义一个List
*/
template<typename T>
struct ListNode{
    ListNode(const T& val ):val_(val),next_(nullptr)
    {
        
    }
    T    val_;
    ListNode* next_;
};
template<typename T>
class List{
    public:
    List()
    {
    }
    List(std::initializer_list<T> l)
    {
        for( auto& e:l )
            insert(e);
    }
    List(ListNode<T>*head )
    {
        while( head )
        {
            insert(head->val_);
            head = head->next_;
        }
       
    }
    ~List()
    {
        auto node = head_;
        while( node )
        {
            auto temp = node->next_;
            delete node;
            node = temp;
        }
        head_ = tail_ = nullptr;
    }
    void insert(T val )
    {
        ListNode<T>* node  = new ListNode<T>(val);
        if( head_ == nullptr  || tail_ == nullptr )
        { 
            head_ = node;
            tail_ = node;
            return;
        }
        tail_->next_ = node;
        tail_ = node;
    }
    void print()
    {
        auto node =head_;
        std::cout<<"[ ";
        while( node )
        {
            auto temp = node->next_;
            std::cout<<node->val_<<" ";
            node = temp;
        }
        std::cout<<"]"<<std::endl;;
    }
    bool empty()
    {
        return head_ == nullptr;
    }
    ListNode<T>* head()
    {
        return head_;
    }
    void erase(ListNode<T>* node )
    {
        if( !node )
            return;
        ListNode<T> **parent = &head_, *start = head_;
        while( start && start != node )
        {
            parent = &(start->next_);
            start =*parent;
        }

        if( !start )
            return;
        *parent = node->next_;//这里就不用处理删除尾元素 头元素的情况了
        if( node->next_ == nullptr )
        {
            //这里有意思了，这里的*parent是最有一个节点的next指针，而tail实际应该等于最有一个节点的指针的
            //所以需要把 next的地址转化成 整个结构的地址，即把某个成员的地址转化成整个结构体的地址
            //鼎鼎大名的内核宏，计算偏移量
            tail_ = *( parent- (size_t)(&(((ListNode<T>*)0)->next_)));
        }
            
        delete node;      
    }
    private:
        ListNode<T>* head_ = nullptr;
        ListNode<T>* tail_ = nullptr;
};
void ListTest()
{
       List<int> l={1,2,3,4,5,6,7};
       auto node = l.head();
       l.print();
       l.erase(node);
       l.print();
       while(node->next_)
       {
           node = node->next_;
       }
       l.erase(node);
       l.print();
       l.insert(100);
       l.insert(200);
       l.print();
}
/*
 *
 *  栈相关算法实现，这里实现一个栈，基于数组实现
 * 
 * 
 */
template<typename T>
class Stack{
    public:
    constexpr static size_t DefaultAllocSize=32;
    Stack()
    {
        data_ = new T[DefaultAllocSize]();
        assert(data_);
        size_ = 0;
        capacity_=DefaultAllocSize;
    }
    ~Stack()
    {
        delete []data_;
        size_   = 0;
        capacity_=0;
    }
    bool empty()
    {
        return size_ == 0;
    }
    void push(const T&val )
    {
        if( size_>=capacity_ )
            resize();
        data_[size_++] = val;
    }
    const T& top()
    {
        if(empty())
            throw std::runtime_error("stack is empty!! when top ");
        return data_[size_-1];
    }
    void pop()
    {
        if(empty())
            throw std::runtime_error("stack is empty!! when pop");
        data_[--size_].~T();
    }
    private:
    void resize()
    { 
        T* temp = new T[capacity_*2]();
        assert(temp);
        std::copy(data_, data_+size_, temp );
        delete []data_;
        data_ = temp;
        capacity_*=2;
    }
    size_t     capacity_;
    size_t     size_;
    T*         data_;
};
void stack_test()
{
    Stack<int> st;
    for( int i=0;i<1000;i++ )
        st.push(i);
    std::cout<<st.top()<<std::endl;
    assert(st.top() == 999 );
    while(!st.empty())
        st.pop();
    std::cout<<"stack test end "<<std::endl;
}
/*
    二叉堆，优先级队列相关的算法实现用这个,基于数组实现，二叉堆完全二叉树 都是基于数组实现
    默认比较是std::less （a<b) 实现大顶堆 
*/

template<typename T,typename CMP=std::less<T>>
class PriorityQueue{
    public:
    constexpr static size_t DefaultAllocSize = 32;
    PriorityQueue():cmp_(CMP())
    {
        data_ = new T[DefaultAllocSize]();
        size_ = 0;
        capacity_ = DefaultAllocSize;
    }
    PriorityQueue(const CMP& cmp):cmp_(cmp)
    {
        data_ = new T[DefaultAllocSize]();
        size_ = 0;
        capacity_ = DefaultAllocSize;
    }
    PriorityQueue( std::initializer_list<T>l ):cmp_(CMP())
    {
        // PriorityQueue();
        // for( auto&& e:l )
        //     push(e);
        /*
            直接push 时间复杂度是 nlogn 如果想堆排序一样做个原地建堆，时间复杂对会优化成logn
        */
       size_t  size = l.size();
       if( size & 7 !=0 )
            capacity_ = size+(8-size%8);
       else 
            capacity_ = size;
       capacity_ *=2;
       data_ = new T[capacity_]();
       size_ = size;
       std::copy(l.begin(),l.end(),data_);
       //从左右一个叶子节点的父节点开始向下调整,最后一个叶子节点为 size_-1 父节点为(size_-1)-1/2=size_/2-1
       for( int i=size_/2-1;i>=0;i-- )
            ShiftDown(i);
    }
    bool empty()
    {
        return size_ == 0;
    }
    size_t size()
    {
        return size_;
    }
    void push(const T& val )
    {
        if( size_>= capacity_ )
            resize();
        data_[size_++]=val;
        ShiftUp(size_-1);
    }
    const T& top()
    {
        if( empty() )
            throw std::runtime_error("priorityqueue is empty when top ");
        return data_[0];
    }
    void pop()
    {
        if( empty() )
            throw std::runtime_error("priorityqueue is empty when pop ");
        data_[0].~T();
        std::swap(data_[0],data_[size_-1]);
        size_--;
        ShiftDown(0);
    }
    private:
    //上浮调整，插入的时候用
    void ShiftUp(int beg)
    {
        //当前节点跟父节点比较 比父节点大 替换父节点
        T temp = data_[beg];
        int father = (beg-1)/2;
        while(beg>0)//这里之所以不用parent是因为 当beg等于0时parent也是0 就会死循环！！！
        {
            if( cmp_(temp,data_[father]))
                break;
            data_[beg] = data_[father];
            beg = father;
            father = (beg-1)/2;
        }
        data_[beg]=temp;

    }
    //下沉调整，删除的时候用
    void ShiftDown(int beg)
    {
        //子节点中较大的跟父节点换位置
        T temp = data_[beg];
        int son = 2*beg+1;
        while( son<size_ )
        {
            if( son+1<size_ &&cmp_(data_[son],data_[son+1]))
                son +=1;//两个孩子中的较大者
            if(!cmp_(temp,data_[son]))
                break;
            data_[beg] = data_[son];
            beg = son;
            son = 2*beg+1;
        }
        data_[beg] = temp;
    }
    void resize( )
    {
        T* temp = new T[capacity_*2]();
        assert(temp);
        std::copy(data_,data_+size_,temp);
        delete []data_;
        data_ = temp;
        capacity_*=2;
    }
    T*          data_;
    size_t      size_;
    size_t      capacity_;
    CMP         cmp_;

};
void priority_queue_test()
{
    PriorityQueue<int> q={90,1,1000,-1,20,3,-10,1};
    while(!q.empty())
    {
        std::cout<<q.top()<<" ";
        q.pop();
    }
    std::cout<<std::endl;
    int number=1000;
    for( int i=0;i<number;i++ )
        q.push(i);
    std::cout<<"push end "<<std::endl;
    for( int i=number-1;i>=0;i-- )
    {
        assert(i == q.top() );
        q.pop();
    }
    std::cout<<__FUNCTION__<<" test end "<<std::endl;
}
#endif