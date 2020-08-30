#ifndef __COMMONDATASTRUCT__HPP__
#define __COMMONDATASTRUCT__HPP__
/*
    算法用到的所有的数据结构实现 链表 栈 堆 
*/
/*
    链表相关算法实现，这里定义一个List
*/
#include <initializer_list>
#include<iostream>
#include<cassert>
#include<algorithm>
#include<functional>
#include<cstdint>
#include<limits.h>
#include<vector>

template<typename T>
struct ListNode{
    ListNode(const T& val ):val_(val),next_(nullptr)
    {
        
    }
    ListNode()
    {}
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
        //这里构造有点问题,如果l是递增的,这里按照每次往头节点插入,最终下来链表变成递减的了~~~~
        for( auto& e:l )
            insert(e);
        reverse();
    }
    List(ListNode<T>*head )
    {
        //这里也一样~~~ head原来如果是递增的,这里按照头节点插入法,最终变成了递减的了~~~
        while( head )
        {
            insert(head->val_);
            head = head->next_;
        }
        reverse();
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
        head_  = nullptr;
    }
    //链表反转
    void reverse()
    {
        decltype(head_) before=nullptr;
        while( head_ )
        {
            auto temp = head_->next_;
            head_->next_ = before;
            before = head_;
            head_ = temp;
        }
        head_ = before;
    }
    void insert(T val )
    {
        ListNode<T>* node  = new ListNode<T>(val);
        if( head_ == nullptr  )
        { 
            head_ = node;
            return;
        }
        node->next_ = head_;
        head_ = node;
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
    ListNode<T>*& head__()//这个用来构造临时对象
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
        // if( node->next_ == nullptr )
        // {
        //     //这里有意思了，这里的*parent是最有一个节点的next指针，而tail实际应该等于最有一个节点的指针的
        //     //所以需要把 next的地址转化成 整个结构的地址，即把某个成员的地址转化成整个结构体的地址
        //     //鼎鼎大名的内核宏，计算偏移量
        //     tail_ = *( parent- (size_t)(&(((ListNode<T>*)0)->next_)));
        // }    
        delete node;      
    }
    private:
        ListNode<T>* head_ = nullptr;
};
void ListTest();

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
void stack_test();
/*
    队列相关算法数据结构实现(数组实现)
*/
template<typename T>
class Queue{
    size_t    head_;
    size_t    tail_;
    T         *data_;
    size_t    size_;
    size_t    capacity_;
    constexpr static size_t  DefaultAlloc=4;
    public:
    Queue()
    {
        size_=0;
        tail_ =0;
        head_ =0;
        capacity_ = DefaultAlloc;
        data_ = new T[capacity_]();
    }
    ~Queue()
    {
        delete []data_;
    }
    void print()
    {
        std::cout<<"capacity "<<capacity_<<" size: "<<size_<<std::endl;
        for(int i=0;i<size_;i++)
        {
            size_t idx = (head_+i)%capacity_;
            std::cout<<data_[idx]<<"  ";
        }
        std::cout<<std::endl;
    }
    bool empty()
    {
        return size_ == 0;
    }
    size_t size()
    {
        return size_;
    }
    void push(const T&val )
    {
        if(size_ >= capacity_ )
            resize();
        data_[tail_] = val;
        tail_ = (tail_+1)%capacity_;
        size_++;
        return;
    }
    void pop()
    {
        if(empty() )
            throw std::runtime_error("Queue empty when pop");
        data_[head_].~T();
        head_ = (head_+1)%capacity_;
        size_--;
    }
    const T& front()
    {
        if(empty() )
            throw std::runtime_error("Queue empty when front");
        return data_[head_];
    }
    const T& back()
    {
        if(empty())
            throw std::runtime_error("Queue empty when back");
        size_t idx = (tail_-1+capacity_)%capacity_;
        return data_[idx];
    }
    private:
    void resize()
    {
        T* temp = new T[capacity_*2];
        for( int i=0;i<size_;i++ )
        {
            int idx = (head_+i)%capacity_;
            temp[i]=data_[idx];
        }
        capacity_ *=2;
        head_ =0;
        tail_ = size_;
        delete []data_;
        data_ = temp;
    }
};
void Queue_test();
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
void priority_queue_test();
/*
    二叉树  二叉树相关算法 按照leetcode的风格
    这里的树不是二叉搜索树
    [-10,9,20,null,null,15,7]  建成的树是
         -10
        / \
        9  20
          /  \
         15   7
*/
template<typename T>
struct TreeNode{
        T   val_;
        TreeNode* left_;
        TreeNode* right_;
        TreeNode(const T& val ):val_(val),left_(nullptr),right_(nullptr){}
        void print()//层序遍历
        {
            Queue<TreeNode<T>*> q;
            std::cout<<val_<<" ";
            q.push(left_);
            q.push(right_);
            while(!q.empty() )
            {
                auto node = q.front();
                q.pop();
                if( node ==nullptr )
                {
                    std::cout<<" null ";
                    continue;
                }

                std::cout<<node->val<<" ";
                q.push(node->left_);
                q.push(node->right_);       
            }
        }
};
template<typename T>
class BinaryTree{
    TreeNode<T>    *root_ = nullptr;
    void destruct(TreeNode<T>*node )
    {
        if( node == nullptr )
            return ;
        destruct(node->left_);
        destruct(node->right_);
        delete node;
    }
    public:

    BinaryTree(){
    }
    ~BinaryTree()
    {
        destruct(root_);
    }
    TreeNode<T>* GetRoot()
    {
        return root_;
    }
    //把null节点算作 INT_MIN
    BinaryTree( std::initializer_list<T>ll)
    {
        std::vector<T> l(ll.begin(),ll.end());
        if(l.size() <=0 )
            return;
        if( l[0] == std::numeric_limits<T>::min() )
            return;
        auto nil = std::numeric_limits<T>::min();
        root_ = new TreeNode<T>(l[0]);
        Queue<TreeNode<T>*> q;
        q.push(root_);

        int i=1;
        while( !q.empty() )
        {
            auto temp = q.front();
            q.pop();
            if( i<l.size() && l[i]!= nil )//左孩子
            {
                auto leftchild = new TreeNode<T>(l[i]);
                temp->left_ = leftchild;
                q.push(leftchild);
            }
            i++;
            if( i<l.size() && l[i]!=nil )
            {
                auto rightchild = new TreeNode<T>(l[i]);
                temp->right_ = rightchild;
                q.push(rightchild);
            }
            i++;
        }

    }
    void levelOrder()//层序遍历打印,叶子节点打印 null
    {
        if(root_ == nullptr )
            return;
        Queue<TreeNode<T>*>q;
        q.push(root_);
        std::cout<<"层序遍历"<<std::endl;
        while( !q.empty() )
        {
            auto temp = q.front();
            q.pop();
            if( temp == nullptr )
            {
                std::cout<<" null "<<" ";
                continue;
            }
            std::cout<<temp->val_<<" ";
            q.push(temp->left_);
            q.push(temp->right_);
        }
        std::cout<<std::endl;
    }

};
void binarytree_test();
void main_datastruct_test();
#endif