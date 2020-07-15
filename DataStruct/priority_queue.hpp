#ifndef  __PRIORITY_QUEUE__H
#define  __PRIORITY_QUEUE__H
namespace datastruct{
//二叉堆
//a<b 构造大顶堆 否则构造小顶堆
template<typename T,typename CMP>
class PriorityQueueBase{
    public:
    virtual ~PriorityQueueBase(){}
    virtual void push( const T& )=0;
    virtual void pop()=0;
    virtual T top()const=0;
    virtual size_t size()const=0;
    virtual bool empty()const=0;
};
//数组描述
template<typename T,typename CMP>
class ArrayPriorityQueue:public PriorityQueueBase<T,CMP>{
    public:
    static constexpr size_t DefaultSize=32;
    static constexpr size_t ResizeLimit=1024;
    ArrayPriorityQueue();
    ~ArrayPriorityQueue();
    void push( const T& )override;
    void pop()override;
    T top()const override;
    size_t size()const override;
    bool empty()const override;
    private:
    //扩容
    void resize(size_t );
    //下沉 pop操作之后调整
    void shiftdown(size_t pos );
    //上浮 push操作之后调整
    void shiftup(size_t pos );
    T       *data_;
    size_t  size_;
    size_t  capacity_;
    CMP     cmp_;
};
template<typename T,typename CMP>
ArrayPriorityQueue<T,CMP>::ArrayPriorityQueue()
{
    data_ = nullptr;
    size_ = capacity_ = 0;
}
template<typename T,typename CMP>
ArrayPriorityQueue<T,CMP>::~ArrayPriorityQueue()
{
   if(data_) 
       delete []data_;
    size_ = capacity_=0;
}
template<typename T,typename CMP>
size_t ArrayPriorityQueue<T,CMP>::size()const
{
    return size_;
}
template<typename T,typename CMP>
bool ArrayPriorityQueue<T,CMP>::empty()const
{
    return size_==0 || data_==nullptr;
}
template<typename T,typename CMP>
T ArrayPriorityQueue<T,CMP>::top()const
{
    if(empty())
        throw std::runtime_error("priority queue is empty");
    return data_[0]; 
}
template<typename T,typename CMP>
void ArrayPriorityQueue<T,CMP>::resize(size_t len)
{
    if( len<=capacity_ )
        return;
    int l = capacity_>ResizeLimit?(capacity_+ResizeLimit):(capacity_?capacity_*2:DefaultSize);
    if( l<len )
        l = len;
    T* tmp = new T[l]();
    assert(tmp);
    std::copy( data_, data_+size_, tmp ); 
    if(data_)
        delete []data_;
    data_ = tmp;
    capacity_ = l;
}
template<typename T, typename CMP>
void ArrayPriorityQueue<T,CMP>::push( const T& val )
{
    resize(size_+1);
    data_[size_++]=val;
    shiftup(size_-1);//上浮
}
template<typename T,typename CMP>
void ArrayPriorityQueue<T,CMP>::pop()
{
    if(empty())
        throw std::runtime_error("priority queue is empty");
    data_[0]=data_[--size_];
    shiftdown(0);
}
/*
 *  上浮  当前子节点坐标为n 父节点坐标为(n-1)/2
 *  缓存pos的值 找到合适的位置 放上去
 *  cmp(a,b) a<b为大顶堆
 * */
template<typename T,typename CMP>
void ArrayPriorityQueue<T,CMP>::shiftup(size_t pos )
{
    T val=data_[pos];
    int son = pos;
    int parent = (son-1)/2;

    while( son>0 )
    {
        if( cmp_(val,data_[parent]))
            break;
        data_[son]=data_[parent];
        son = parent ; 
        parent = (son-1)/2;
    }
    data_[son]=val;
}
/*
 *  下沉
 *  当前节点下标为n 则左孩子为2n+1 右孩子为2n+2
 * */
template<typename T,typename CMP>
void ArrayPriorityQueue<T,CMP>::shiftdown(size_t pos )
{
    T val = data_[pos];
    int parent = pos;
    int child = 2*pos+1;
    while( child<size_ )
    {
        if( child+1<size_ && cmp_(data_[child],data_[child+1]) )
            child+=1;
        if( cmp_( data_[child], val ))
            break;
        data_[parent]=data_[child];
        parent = child;
        child = 2*parent+1;
    }
    data_[parent]=val;
}
}//namespace datastruct
#ifdef _UNIT_TEST_
void priority_queue_test(int argc,char**argv );
#endif
#endif
