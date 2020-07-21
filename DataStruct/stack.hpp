#ifndef __STACK__H
#define __STACK__H
#include<cstdint>
#include<cstdio>
#include<cassert>
#include<algorithm>
#include<stdexcept>
/*
 *  栈 
 *  先进后出 后进先出
 *  这个可以由数组实现,也可以链表实现,数组实现push的时候需要注意resize操作,适当的2倍增加长度
 *
 *  每种实现实现一个迭代器 当是练练手了,stl的迭代器是每种容器实现了一种迭代器 然后
 *
 *  链表则无此限制
 * */


namespace datastruct{
template<typename T>
class ArrayStack;
template<typename T>
class ArrayIterator{
    friend  ArrayStack<T>;
    public:
    ArrayIterator( T*ptr = nullptr ):current_(ptr){}
    T operator*()const{
        return *current_;
    }
    T operator->()const{
        return current_;
    }
    //前++
    ArrayIterator& operator++()
    {
        current_+=1;
        return *this;
    }
    //后++
    ArrayIterator& operator++(int)
    {
        T *tmp = current_;
        current_+=1;
        return ArrayIterator(tmp);
    }
    bool operator==(const ArrayIterator& other)
    {
        return current_ == other.current_;
    }
    bool operator !=(const ArrayIterator&other)
    {
        return current_ !=other.current_;
    }

    private:
    T       *current_;
};
template<typename T>
class StackBase{
public:
    virtual ~StackBase()=0;
    virtual bool    IsEmpty()const=0;
    virtual size_t  Size()const=0;
    virtual T Top()const =0;
    virtual void Pop()=0;
    virtual void Push( const T& )=0; 
    //这个size_ 本来是可以放在基类的 作为protected成员 及防治了外部访问又不影响子类使用,但是模板类这里有问题 子类访问的时候需要带上this指针!不知道为什么
};
//基类析构函数设置成纯虚函数,可以防止如果没有其他纯虚函数的时候,对基类实例化
//但是基类的析构函数必须有实现,因为子类析构的时候必须要能调用基类的析构函数
template<typename T>
StackBase<T>::~StackBase(){}
//数组描述
template<typename T>
class ArrayStack:public StackBase<T>{
public:
    
    static constexpr size_t  default_size=32;
    static constexpr size_t  realloclimit=1024;
    ArrayStack(size_t len );
    ArrayStack();
    ~ArrayStack();

    bool IsEmpty()const override;
    size_t Size()const override;
    T Top()const override;
    void Pop()override;
    void Push(const T& )override;

    using iterator = ArrayIterator<T>;

    iterator begin()const{
        return iterator(ptr_);
    }
    iterator end()const{
        return iterator(ptr_+size_);
    }
private:
    T         *ptr_ = nullptr;
    size_t    capcity_;
    size_t    size_;
};
template< typename T>
ArrayStack<T>::ArrayStack()
{
   ptr_ = new T[default_size]();
   assert( ptr_ );
   size_ =0;
   capcity_=default_size;  
}
template<typename T>
ArrayStack<T>::ArrayStack(size_t len )
{
    len = len<default_size?default_size:len;
    ptr_ = new T[len]();
    size_ = 0;
    capcity_=len;
}
template<typename T>
ArrayStack<T>::~ArrayStack()
{
    delete []ptr_;
    capcity_ = size_ = 0;
}
template<typename T>
bool ArrayStack<T>::IsEmpty()const
{
    return size_==0;
}
template<typename T>
size_t ArrayStack<T>::Size()const
{
    return size_;
}
template<typename T>
T ArrayStack<T>::Top()const
{
    if(IsEmpty())
        throw std::runtime_error("stack is empty");
    return ptr_[size_-1];
}
template<typename T>
void ArrayStack<T>::Pop()
{
    if(IsEmpty())
        throw std::runtime_error("stack is empty");
    ptr_[--size_].~T();
}
template<typename T>
void ArrayStack<T>::Push(const T& val )
{
    if( capcity_<size_+1 )
    {
        capcity_ = capcity_<realloclimit?capcity_*2:capcity_+realloclimit;
        T *tmp = new T[capcity_]();
        assert(tmp);
        std::copy( ptr_, ptr_+size_, tmp );
        delete []ptr_;
        ptr_ = tmp; 
    }
    ptr_[size_++]=val;
}
//链表描述
template<typename T>
class LinkedListStack:public StackBase<T>{
    public:
        LinkedListStack();
        ~LinkedListStack();
        bool    IsEmpty()const override;
        size_t  Size()const override;
        T       Top()const override;
        void    Pop()override;
        void    Push(const T& val )override;
         
    private:
        struct Node{
            T  val_;
            Node* next_ = nullptr;
        };
    Node*   head_;
    size_t  size_;
};
template<typename T>
LinkedListStack<T>::LinkedListStack()
{
    size_ =0;
    head_ = nullptr;
}
template<typename T>
LinkedListStack<T>::~LinkedListStack()
{
    size_ = 0;
    Node* del = head_;
    while( del )
    {
        Node *temp = del->next_;
        delete del;
        del = temp;
    }
    head_ = nullptr;
}
template<typename T>
bool LinkedListStack<T>::IsEmpty()const
{
    return size_ == 0 || head_ == nullptr;
}
template<typename T>
size_t LinkedListStack<T>::Size()const
{
    return size_;
}
template<typename T>
T LinkedListStack<T>::Top()const
{
    if( IsEmpty() )
        throw std::runtime_error("stack is empty");
    return head_->val_;
}
template<typename T>
void LinkedListStack<T>::Pop()
{
    if( IsEmpty() )
        throw std::runtime_error("stack is empty");
    Node* temp = head_;
    head_ = head_->next_;
    size_--;
    delete temp;
}
template<typename T>
void LinkedListStack<T>::Push( const T& val )
{
    Node* node = new Node();
    assert( node );
    node->val_=val;
    node->next_ = head_;
    head_ = node;
    size_++;
}
}//namespace datastrct end

#endif
