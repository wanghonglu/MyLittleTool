#ifndef  __PRIORITY_QUEUE__H
#define  __PRIORITY_QUEUE__H
#include<functional>
namespace datastruct{
//二叉堆
//a<b 构造大顶堆 否则构造小顶堆
//   优先级队列 本质上是二叉树,但是因为它是完全二叉树( 跟满二叉树对比,节点从左到右编号,所有节点都能跟满二叉树对上)
//   所以对内存空间的利用比较充分,而且通过下标寻址也相对简单,所以一般是用数组实现,链表实现的比较麻烦,这里就不实现了
//   传入比较可调用对象,这里如果是a<b返回,就是最大堆 否则是最小堆,默认是最大堆跟stl::priority_queue对齐
//
//   通过坐标运算就能获取到父节点 子节点是数组实现的根本原因
//   当前节点坐标为k 则它左孩子为2k+1 右孩子为2k+2 父节点坐标为(k-1)/2
//   二叉堆有三个操作要尤其注意
//
//   上浮/下沉/原地建堆(多在堆排序的时候用)
//
//   上浮:
//      每次插入一个节点,都是插在数组的最后,以满足完全二叉树的性质,然后开始上浮
//      即跟当前父节点比较是否满足二叉堆,如果满足 满足就跳出将当前节点放在子节点的位置,负责将父节点放在子节点的位置
//      继续往上找,直到子节点 满足就跳出将当前节点放在子节点的位置,负责将父节点放在子节点的位置
//      继续往上找,直到子节点不大于0  这里不用父节点做判断条件是,因为只有一个节点的时候 第一步索引出来的父节点就是0 但是这里需要判断
//      写的时候多想想
//   下沉:
//      当删除堆顶的时候,需要把最后的节点放在堆顶,然后跟孩子节点对比,大顶堆找出两个孩子中最大的那一个,然后跟父节点对比,是否满足堆特性
//      满足就安排在父节点,负责继续往下,这里的边界时子节点最大时len-1
//
//   原地建堆:
//      这里采用的时自下而上的下沉操作,有点绕,反正就是找到最后一个父节点,然后对该子树做下沉操作,二叉堆越往下就调整的越少
//      这样的时间复杂度是O(n)
//      n/2是不需要调整的 n/4是需要调整一步的  .... S = 0*n/2+ 1*n/4+2*n/8 ....  2S-S 就能算出O(n) (n步,系数是1/4 .... 系数也比较小
//
//      默认模板参数 在定义的时候写就行了,实现的时候不用写
//
template<typename T,typename CMP=std::less<T>>
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
template<typename T,typename CMP=std::less<T>>
class ArrayPriorityQueue:public PriorityQueueBase<T,CMP>{
    public:
    static constexpr size_t DefaultSize=32;
    static constexpr size_t ResizeLimit=1024;
    ArrayPriorityQueue();
    //建堆 从最后一个节点的父节点开始向下调整，类似堆排序，这里的时间复杂度是O(n)
    ArrayPriorityQueue(const T*array, size_t );
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
/* 建堆，这里采取的是从最后一个父节点开始 往下调整，这样做时间复杂度O(n)
 * 以为次下层的节点是N/4 需要移动1步 节点越多移动的越少 越高效
 * 最后一个节点是len-1,那么他的父节点就是(len-1-1)/2 即len/2-1，然后依次--调整
 * */
template<typename T,typename CMP>
ArrayPriorityQueue<T,CMP>::ArrayPriorityQueue(const T*array, size_t len )
{
    data_=nullptr;
    size_ = capacity_ = 0;
    
    resize( len ); 
    std::copy(array, array+len, data_ );
    size_ = len;
    for( int i=len/2 -1;i>=0;i-- )
        shiftdown(i); 

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
#endif
