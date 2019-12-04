/*
 * 单例类 不可拷贝构造和赋值
 * */
#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__
template<typename T>
class Singleton{
    public:
        static T& Instance(){
            static T t;
            return t;
        }
    private:
        Singleton()=default;
        Singleton( const Singleton& )=delete;
        Singleton& operator = ( const Singleton&  )=delete;

};
#endif
