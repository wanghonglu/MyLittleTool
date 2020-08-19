#ifndef __THREADSIGLETON__H
#define __THREADSIGLETON__H
#include<pthread.h>
template<typename T>
class Singleton{
    public:
    static T& Instance()
    {
        pthread_once(&ponce_, &Singleton::Init );
        return *value_;    
    }
    private:
    Singleton(){}
    ~Singleton(){}

    static void Init()
    {
        value_ = new T();
    }
    private:
    static pthread_once_t ponce_;
    static T* value_;

};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;
//这种实现叫做懒汉模式，就是用的时候再构造
//公司用的那种叫饥汉模式，即先构造后用，这种还是线程安全的 double check 也是不太行的 volatile有可能会乱序
#endif
