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
#endif
