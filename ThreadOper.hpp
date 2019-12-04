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
#include<sys/syscall.h>
#include<unistd.h>
template< typename T>
class ThreadOper{
    explicit ThreadOper()
    {
        m_listDoing = std::make_unique<std::list<T>>();
        m_listWait = std::make_unique<std::list<T>>();
        m_stop = false;
    }

    void    Start()
    {
        m_thread.reset([](){
                LoopRun();
                });
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
        m_stop = true;
    }

    void    Join()
    {
        if( m_thread.get() && m_thread->joinable() )
            m_thread->join();
    }

    inline bool IsStoped()const 
    {
        return m_stop;
    }
    static unsigned int GetThreadId()
    {
        static thread_local  unsigned int s_threadId=0;
        if( s_threadId !=0  ) return s_threadId;
        return s_threadId = syscall(SYS_gettid);
    }
    private:
    void Swap()
    {
        std::unique_lock<std::mutex> l(m_mutex);
        m_condition.wait(l,[this]()->bool{ return m_stop || !m_listWait->empty(); });
        if( m_stop  )
            return;
        m_listDoing.swap( m_listWait );
        return;
    }
        mutable std::mutex               m_mutex;
        std::condition_variable          m_condition;
        std::unique_ptr<std::list<T>>    m_listDoing;
        std::unique_ptr<std::list<T>>    m_listWait;
        std::shared_ptr<std::thread>     m_thread;
        std::atomic<bool>                m_stop = false;
};
#endif
