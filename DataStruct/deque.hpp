#ifndef __DEQUE__H
#define __DEQUE__H
#include<iostream>
/*
 *
 *  双端队列
 *
 *      实现可以两端插入和删除 
 *      数组实现 可以跟循环队列一样 浪费2个空间来判断空判断满,感觉麻烦,直接添加一个属性size_
 *      注意在队列为空的时候push的时候判断下为空的情况
 *
 *      注意这个其实很低效 因为在数组扩容的时候 牵扯到数组的拷贝
 *      std::deque的设计时采用了一个二维数组,每次扩容 new出一片新的空间存放数组,设计还挺好的
 *      有空了练练手
 *      数组实现 要注意resize之后 前后指针的位置也要改 
 *
 *      链表实现采用双向链表
 *      需要注意两个点,第一 队列空的时候 赋值需要同时把fron_和rear_都赋值
 *                     第二 pop操作的时候 如果队列为空了,此时的front_和rear_指针是指向同一个对象的(即size_==1的时候)
 *                     此时delete 其中一个 另一个就变成野指针了,所以需要在为空的情况下 将另一个也置空
 * */
template<typename T>
class DequeBase{
    public:
    virtual ~DequeBase(){}

    virtual bool empty()const=0;
    virtual size_t size()const=0;
    virtual T    front()const=0;
    virtual T    back()const =0;

    virtual void push_back( const T& )=0;
    virtual void push_front(const T& )=0;
    virtual void pop_back()=0;
    virtual void pop_front()=0;
};
template<typename T>
class ArrayDeque:public DequeBase<T>{
    public:
    static constexpr size_t DefaultSize=16;
    static constexpr size_t ResizeLimit=1024;
    ArrayDeque()
    {
        data_ = new T[DefaultSize]();
        size_=0;
        front_ = 0;//能放下一个元素的位置
        rear_ = 0;//能放下一个元素的位置
        capacity_=DefaultSize;
    }
    ~ArrayDeque()
    {
        if(data_)
            delete []data_;
    }
    bool empty()const override
    {
        return size_ == 0;
    }
    size_t size()const override
    {
        return size_;
    }
    T front()const override
    {
        if( empty() )
            throw std::runtime_error( "deque is empty" );
        return data_[(front_+1)%capacity_];
    }
    T back()const override 
    {
        if( empty() )
            throw std::runtime_error( "deque is empty" );
        return data_[(rear_-1+capacity_)%capacity_];
    }
    void push_front( const T& val )override
    {
        resize();
        if( empty() && front_ == rear_ )//空队列的时候稍微需要处理下
        {
            rear_ = (rear_+1)%capacity_;
        }
        data_[front_] = val;
        front_ = (front_-1+capacity_)%capacity_;
        size_++;
    }
    void push_back( const T& val )override
    {
        resize();
        if( empty()&& front_ == rear_ )//push的时候队列空的时候需要单独处理下
        {
            front_ = (front_-1+capacity_)%capacity_;
        }
        data_[rear_]=val;
        rear_ = (rear_+1)%capacity_;
        size_++;
    }
    void pop_front()override
    {
        if( empty() )
            throw std::runtime_error( "deque is empty" );
        front_ = (front_+1)%capacity_;
        data_[front_].~T();
        size_--;
    }
    void pop_back()override
    {
        if( empty() )
            throw std::runtime_error( "deque is empty" );
        rear_ = (rear_-1+capacity_)%capacity_;
        data_[rear_].~T();
        size_--;
    }
    private:
    void resize()
    {
        if( size_<capacity_ )
            return;
        int len = capacity_>ResizeLimit?(capacity_+ResizeLimit):(capacity_?2*capacity_:DefaultSize);
        T *tmp = new T[len]();
        size_t beg = (front_+1)%capacity_;
        for( int i=0;i<size_;i++ )
        {
            tmp[i] = data_[(beg+i)%capacity_];
        }
        delete []data_;
        capacity_ = len;
        front_=capacity_-1;
        rear_=size_;
        data_ = tmp;
    }
    T        *data_ = nullptr;
    size_t    size_ = 0;
    size_t    capacity_=0;
    size_t    front_=0; //指向下一个可以在前面放元素的位置
    size_t    rear_=0; //指向下一个可以在尾部放元素的位置
};
template<typename T>
class LinkedListDeque:public DequeBase<T>{
    public:
        LinkedListDeque()
        {
            size_=0;
            front_=rear_=nullptr;
        }
        ~LinkedListDeque()
        {
            while( front_ )
            {
                Node* temp = front_->next_;
                assert( temp != front_ );
                delete front_;
                front_ = temp;
            }
        }
        bool empty()const override
        {
            return size_ == 0 ;
        }
        size_t size()const override
        {
            return size_;
        }
        T front()const override
        {
            if( empty() )
                throw std::runtime_error("deque is empty");
            return front_->val_;
        }
        T back()const override 
        {
            if( empty() )
                throw std::runtime_error("deque is empty");
            return rear_->val_;
        }
        void push_back(const T& val )override
        {
            Node *node = new Node(val);
            if( empty() )
                front_ = rear_ = node;
            else
            {
                rear_->next_ = node;
                node->pre_ = rear_;
                rear_ = node;
            }
            size_++;
        }
        void push_front( const T&val )override
        {
            Node *node= new Node(val);
            if( empty() )
                front_ = rear_ = node;
            else
            {
                front_->pre_ = node;
                node->next_ = front_;
                front_ = node;
            }
            size_++;
        }
        void pop_back()override
        {
            if( empty() )
                throw std::runtime_error("deque is empty");
            Node* temp = rear_->pre_;
            delete rear_;
            rear_ = temp;
            if( rear_ )
                rear_->next_ = nullptr;
            size_--;

            //当元素为1的时候 这时候 rear_其实和front_是指向同一个对象的 但是这里把rear_改为了nullptr
            //并且把对象释放掉了 front_就变成一个空指针了
            if( empty() )
                front_ = nullptr;
        }
        void pop_front()override
        {
            if( empty() )
                throw std::runtime_error("deque is empty");
            Node* temp = front_->next_;
            delete front_;
            front_ = temp;
            if( front_ )
                front_->pre_ = nullptr;
            size_--;
            //原因童 pop_back
            if( empty() )
                rear_ = nullptr;
        }
    private:
    struct Node
    {
        Node* pre_=nullptr;
        Node* next_=nullptr;
        T     val_;
        Node( const T& val):val_(val),pre_(nullptr),next_(nullptr){}
    };
    Node  *front_=nullptr;
    Node  *rear_=nullptr;
    size_t size_=0;
}; 
#endif
