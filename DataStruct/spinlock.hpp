#include<atomic>
/*
    自旋锁适用于,操作间隔时间短,根mutex比起来,自旋不会切换线程,可能会加剧cpu的消耗
    耗费在旋转上,如果操作特别简单 自旋锁应该性能不错
*/
class Spinlock{
public:
    Spinlock():flag(ATOMIC_FLAG_INIT)
    {
        
    }
    void lock()
    {
        while( flag.test_and_set())//如果为真则返回true,否则设置为true 并返回false
        ;
    }
    void unlock()
    {
        flag.clear();
    }
private:
    std::atomic_flag   flag;//这个是真正的无锁 
};
