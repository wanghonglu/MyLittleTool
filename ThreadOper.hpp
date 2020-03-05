/*
 * 基于条件变量的线程模型
 * */
#ifndef __ThreadOper_HPP__
#define __ThreadOper_HPP__
#include<thread>
#include<condition_variable>
#include<memory>
#include<list>
#include<vector>
#include<atomic>
#include<cassert>
#ifdef __linux__ 
#include<unistd.h>
#include <sys/prctl.h>
#endif
#include<chrono>
template< typename T>
class ThreadOper{
    public:
    explicit ThreadOper()
    {
        m_listDoing = std::make_shared<std::list<T>>();
        m_listWait = std::make_shared<std::list<T>>();
        m_stop.store(false);
    }

    void    Start()
    {
        m_thread.reset( new std::thread ([this](){
                LoopRun();
                }));
    }

    virtual  void LoopRun()=0;//子类实现

    void    Push( const T& t)
    {
        std::unique_lock< std::mutex > l( m_mutex  );
        m_listWait->push_back( t );
        m_condition.notify_one();
    }


    void    Stop()
    {
        m_stop.store(true);
    }

    void    Join()
    {
        if( m_thread.get() && m_thread->joinable() )
            m_thread->join();
    }
    void    Sleep( uint32_t ms )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(ms) );
    }

    void   SignNameForThread( const std::string threadname )
    {
#ifdef __linux__ 
        prctl(PR_SET_NAME, threadname.c_str());
#endif
    }

    inline bool IsStoped()const 
    {
        return m_stop.load() == true;
    }
    std::shared_ptr<std::list<T>>&  Pop()
    {
        std::unique_lock<std::mutex> l(m_mutex);
        m_condition.wait(l,[this]()->bool{ return m_stop || !m_listWait->empty(); });
        if( m_stop  )
            return m_listDoing;
        m_listDoing.swap( m_listWait );
        return m_listDoing;
    }
    private:
        mutable std::mutex               m_mutex;
        std::condition_variable          m_condition;
        std::shared_ptr<std::list<T>>    m_listDoing;
        std::shared_ptr<std::list<T>>    m_listWait;
        std::shared_ptr<std::thread>     m_thread;
        std::atomic<bool>                m_stop;
};
#endif
