#ifndef __TimeWheel__H
#define __TimeWheel__H

//时间轮算法  模仿内核的时间轮实现
//用五个数组 v1 255个槽 剩下的4个每个63个槽
//设计是 用32位的int整数 低8位代表 第一个轮子  9到14位代表第二个轮子
//第一个轮子每个代表1ms 第一个轮子转255 第二个轮子转1圈,第二个轮子转63圈 第三个轮子转一圈
//依次类推
#include<time.h>
#include<stdint.h>
#include<pthread.h>
#include <sys/time.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<unistd.h>

#define TV1_BIT 8
#define TV_BIT  6

#define TV1_SIZE  (1<<TV1_BIT)
#define TV_SIZE   (1<<TV_BIT) 
#define TV1_MASK (TV1_SIZE-1)
#define TV_MASK  (TV_SIZE-1)
//取出每一级的时间轮 的具体值
//这个在 thread_local的调度器上 所以不用加锁
//可以用信号来做过度 也可以用select /epoll_wait 这里我们留出接口用 epoll_wait来做tick
//
typedef enum TimerOpt {
    Exec_Once,
    Exec_All,
    Exec_Times,
} TimerOpt;
typedef void*(*timeout_handle)(void*);
struct TimerList;
typedef struct TimerOptions{
    time_t          timeout_;
    uint32_t        execu_times_;// flag 为2时的执行次数 
    unsigned char   flag_:2; //0-执行1次 1-重复执行 2- 执行n次
}TimerOptions;
typedef struct Timer{
    timeout_handle  handle_;//超时处理函数
    time_t          expried_time_;//到期时间
    void*           data_;//附加数据
    TimerOptions    timer_options_;
    struct Timer* next_;
    struct Timer* pre_;
    struct TimerList* linked_list_;//timer 所在的链表
}Timer;
//单向链表删除也可以O(1) 就是把当前值替换成 next节点的值 然后把当前的next指针指向 next->next指针
//这样 就可以相当于删除链表的节点 但是需要注意是否是最后一个节点
//单项链表删除可以不用从前往后遍历的
//但是如果删除的是尾节点还是得老老实实的从头开始找 好在平均时间复杂度是满足要求的
//但有个弊端 这样会修改待删除的节点,这里其实是不想要修改的,因为可能的情况是从一个链表转移到另一个链表
//所以这里只能双端链表
typedef struct TimerList{
    Timer* head_;
    Timer* tail_;
}TimerList;
//毫秒为单位 最长可以记录49天的超时 够用了
typedef struct TimeWheel{
    TimerList       tv1_[256]; 
    TimerList       tv2_[64];  
    TimerList       tv3_[64];  
    TimerList       tv4_[64];  
    TimerList       tv5_[64];  
    uint64_t        tick_;//当前的毫秒值
    int             idx_;//当前刻度指针所在的位置
}TimeWheel;

TimeWheel* InitTimeWheel();
uint64_t now_milliseconds();
//opt-0 只执行一次 opt-1循环执行 opt-2 执行n次  excute_times flag为2时 执行的次数 
Timer* AddTimer(TimeWheel* timewheel, timeout_handle handler, uint64_t time_out,void*data,TimerOpt opt,uint32_t excute_times );
Timer* _AddTimer(TimeWheel*, Timer* );
void Tick(TimeWheel* );
//触发其他轮子上的定时器
void SwitchOthWheel(TimeWheel*wh);
//迁移定时器到下一级轮子上
void _SwitchOthWheel(TimeWheel*wh, TimerList* l,size_t idx  );
void freeTimer( Timer*  );

void AddTimerList( TimerList* l, Timer* timer );
void DeleteTimerList(Timer* timer );
#endif
