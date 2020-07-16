#ifndef __QUEUE__H
#define __QUEUE__H
/*
 *  队列
 *  先进先出的结构,数组实现和链表实现均可
 *  数组实现需要记录前后位置,front 和rear 判空可以通过 front == rear
 *  front是当前队列头的下标,rear是队列尾元素的下一个位置 (即下一个可放元素的位置) 判满可以用 (readr+1)%capcity来判断是否满
 *  这里其实牺牲掉了一个元素,比如长度16的队列话 存放15个元素就放不下了( 15个元素的时候rear为15 此时(15+1)%capacity == front
 *  这里数组扩容还是采用 2倍的方式,这样的话 取余操作就能写成 &操作
 *  假如头指针在1  尾指针在0 那么数列的长度为( rear-front+Size)%Size =15 
 *
 *  扩容也挺麻烦的
 *  假如原来数组长度16 此时rear是2 front是3,此时是满了(其实位置2还是空的)
 *  扩容就需要把这15个元素拷贝到新的数组里面,然后需要重置 front rear capacity_这些属性
 *
 *  这里实现的是单向循环队列
 *  这里牺牲了一个元素来判断队列是否满
 *
 * */
namespace datastruct{
template<typename T>
class QueueBase{
    public:
    virtual ~QueueBase(){}
    virtual size_t size()const=0;
    virtual bool   empty()const=0;
    virtual T&     front()=0;
    virtual T&     back()=0;
    virtual T      front()const=0;
    virtual T      back()const=0;

    virtual void   push(const T& )=0;
    virtual void   pop()=0;
};
//数组实现
template<typename T>
class ArrayQueue:public QueueBase<T>{
    public:
    static constexpr size_t DefaultSize=16;
    static constexpr size_t ResizeLimit=1024;
    ArrayQueue()
    {
        data_ = nullptr;
        front_ = rear_ =capacity_=0;
        //构造的时候一定先扩容 防止除0奔溃
        data_  = new T[DefaultSize]();
        capacity_ = DefaultSize;
    }
    ~ArrayQueue()
    {
        if(data_)
            delete []data_;
        front_ = rear_ =capacity_=0;
    }
    //长度计算 (rear_ - front+capacity_)%capacity_
    size_t size()const override
    {
        return (rear_ - front_ +capacity_)%capacity_;
    }
    bool empty()const override
    {
        return rear_ == front_;
    }
    bool full()const
    {
        return (rear_+1)%capacity_ == front_;//牺牲掉一个元素 方便判满
    }
    T& front() override
    {
        if( empty())
            throw std::runtime_error("queue is empty");
        return data_[front_];
    }
    T& back() override
    {
        if( empty())
            throw std::runtime_error("queue is empty");
        return data_[rear_-1];
    }
    T front()const override
    {
        if( empty())
            throw std::runtime_error("queue is empty");
        return data_[front_];
    }
    T back()const override
    {
        if( empty())
            throw std::runtime_error("queue is empty");
        return data_[rear_-1];
    }
    /*
     *  2倍扩容 超过1024字节就在当前字节上+1024 防止占用太多
     *  扩容后要拷贝当前数组内的元素到新数组
     * */
    void resize()
    {
        if( !full() )//容量为0或者当前数组已满才需要扩容
            return ;

        int len = capacity_>ResizeLimit?(capacity_+ResizeLimit):(capacity_?2*capacity_:DefaultSize); 
        T *tmp = new T[len]();
        assert(tmp);
        size_t  already_have = size();
        for( int i=0;i<already_have;i++ )
        {
            tmp[i]=data_[(i+front_)%capacity_];
        }
        front_ = 0;
        rear_ = already_have;
        capacity_ = len;
        delete []data_;
        data_ = tmp;
    }
    void push( const T& val )override
    {
       resize();
       data_[rear_] = val;
       rear_ = (rear_+1)%capacity_; 
    }
    void pop()override
    {
        if(empty())
            throw std::runtime_error("queue is empty");
        data_[front_].~T();
        front_ = (front_+1)%capacity_;
        return;
    }
    size_t capacity()const
    {
        return capacity_;
    }
    private:
    T           *data_ = nullptr;
    size_t      front_=0;
    size_t      rear_ = 0; 
    size_t      capacity_=0;
};
/*  链表描述 简单 
 * */
template<typename T>
class LinkedListQueue:public  QueueBase<T>{
    public:
    LinkedListQueue()
    {
        head_ = tail_ = nullptr;
        size_ =0;
    }
    bool empty()const override
    {
        return size_ == 0;
    }
    size_t size()const override
    {
        return size_;
    }
    T   back()const override
    {
        if( empty() )
            throw std::runtime_error( "queue is empty" );
        return tail_->val_;
    }
    T&   back()override
    {
        if( empty() )
            throw std::runtime_error( "queue is empty" );
        return tail_->val_;
    }
    T   front()const override
    {
        if( empty() )
            throw std::runtime_error( "queue is empty" );
        return head_->val_;
    }
    T&   front()override
    {
        if( empty() )
            throw std::runtime_error( "queue is empty" );
        return head_->val_;
    }
    //队尾push
    void push( const T& val )override 
    {
        Node * node = new Node();
        node->val_ = val;
        node->next_ = nullptr;
        if( empty() )
            tail_ = head_ = node;
        else 
        {
            tail_->next_ = node;
            tail_ = node;
        }
        size_++;
    }
    //队头pop
    void pop()override
    { 
        if( empty() )
            throw std::runtime_error( "queue is empty" );
        Node* node = head_;
        head_ = head_->next_;
        size_--;
        delete node;
    }
    
    private:
    struct Node{
        T      val_;
        Node*  next_;
    };
    Node*   head_;
    Node*   tail_;
    size_t  size_;
};
}//namespace datastruct 
#endif
