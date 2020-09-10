#include "timewheel.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

uint64_t now_milliseconds()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000;
}

TimeWheel* InitTimeWheel()
{
    TimeWheel*  wh= (TimeWheel*)malloc(sizeof(TimeWheel));
    memset(wh,0x00, sizeof(TimeWheel));
    wh->tick_=now_milliseconds();
    return wh;
}
Timer* AddTimer(TimeWheel* timer_manage, timeout_handle handler, uint64_t timeout,void*data ,TimerOpt  opt, uint32_t excute_times)
{
    if(timeout<1 )
    {
        fprintf( stderr, "该定时器只支持精度1ms的定时器" );
        return NULL;
    }
    if(opt!=0 && opt!=1 && opt!=2 )
    {
        fprintf( stderr, "opt参数错误 0:只执行一次 1:只能无数次 2:执行指定次数" );
        return NULL;
    }
    Timer* timer=(Timer*)malloc(sizeof(Timer));
    memset(timer, 0x00, sizeof(Timer));
    timer->handle_ = handler;
    timer->data_ = data;
    timer->next_ = NULL;
    timer->pre_ = NULL;
    timer->timer_options_.timeout_ = timeout;
    timer->timer_options_.flag_ = opt;
    timer->timer_options_.execu_times_ = excute_times;
    timer->expried_time_=timer->timer_options_.timeout_+timer_manage->tick_;
    return _AddTimer(timer_manage,timer);
}
/*
 * 取到期时间的 和 当前时间的差值  记作idx
 * 如果idx<256 则取到期时间的第八位 放在 tv1 作为hash槽防止这个timer
 * idx<2^14    放在tv2上
 * */
Timer* _AddTimer(TimeWheel* timer_manage, Timer* timer )
{
    int idx=timer->expried_time_ - timer_manage->tick_;
    if(idx<=0)//这种情况应该立即执行
    {
        timer->handle_(timer->data_);
        if( timer->timer_options_.flag_ == 1 )//todo 优化
        {
            timer->expried_time_ = now_milliseconds()+timer->timer_options_.timeout_;
            _AddTimer(timer_manage,timer);
            return timer;
        }
        else
        {
            free(timer);
            return NULL;
        }
    }
    //32位的int  第八位由第一个时间轮用 后面24位分别由四个时间轮用 每个轮子用4位
    TimerList* list = NULL;
    size_t  real_idx=0;
    //printf("%d \n", idx);
    if( idx<1<<TV1_BIT )//1-255
    {
        //放在tv1上  取低8位作为tv1的数组下标
        real_idx = timer->expried_time_&TV1_MASK;
     //   printf("TV1 %ld expried:%ld now:%ld TV1_MAS:%d  now_idx:%ld\n", real_idx, timer->expried_time_,now_milliseconds(),TV1_MASK,now_milliseconds()&TV1_MASK);
        list = &timer_manage->tv1_[real_idx];
    }
    else if (idx< 1<<(TV1_BIT+TV_BIT))//1小于1左移 14位
    {
        //取出9到14位的值作为tv2_的下标
        real_idx = (timer->expried_time_>>TV1_BIT)&TV_MASK;
      //  printf("TV2 %ld expried:%ld now:%ld TV_MAS:%d  now_idx:%ld\n", real_idx, timer->expried_time_,now_milliseconds(),TV_MASK,now_milliseconds()&TV_MASK);
        list = &timer_manage->tv2_[real_idx];
    }
    else if( idx<1<<(TV1_BIT+2*TV_BIT))
    {
        //取出 15到20位做位 tv3_的下标
        real_idx=(timer->expried_time_>>(TV1_BIT+TV_BIT))&TV_MASK;
       // printf("TV3 %ld expried:%ld now:%ld TV_MAS:%d  now_idx:%ld\n", real_idx, timer->expried_time_,now_milliseconds(),TV_MASK,now_milliseconds()&TV_MASK);
        list = &timer_manage->tv3_[real_idx];
    }
    else if (idx<1<<(TV1_BIT+3*TV_BIT))
    {
        //取出 21到26位给 tv4_的下标
        real_idx=(timer->expried_time_>>(TV1_BIT+2*TV_BIT))&TV_MASK;
        //printf("TV4 %ld expried:%ld now:%ld TV1_MAS:%d  now_idx:%ld\n", real_idx, timer->expried_time_,now_milliseconds(),TV1_MASK,now_milliseconds()&TV1_MASK);
        list = &timer_manage->tv4_[real_idx];
    }
    else
    {
        real_idx=(timer->expried_time_>>(TV1_BIT+3*TV_BIT))&TV_MASK;
        //printf("TV5 %ld expried:%ld now:%ld TV1_MAS:%d  now_idx:%ld\n", real_idx, timer->expried_time_,now_milliseconds(),TV1_MASK,now_milliseconds()&TV1_MASK);
        list = &timer_manage->tv4_[real_idx];
    }
    AddTimerList(list,timer);
    return timer;
}
void AddTimerList(TimerList* l, Timer* timer )
{
   timer->linked_list_ = l;
   if( l->tail_ == NULL )
   {
       l->head_ = l->tail_ = timer;
       timer->pre_ = timer->next_ = NULL;
       return;
   }
   else
   {
       timer->pre_ = l->tail_;
       timer->next_ = NULL;
       l->tail_->next_ = timer;
       l->tail_ = timer;
   }
}
void  DeleteTimerList(Timer* timer )
{
   TimerList* l = timer->linked_list_;
   if( l->head_ == timer  )
   {
       l->head_ = timer->next_;
       if(l->head_)
           l->head_->pre_ = NULL;
   }
   else if( l->tail_ == timer  )
   {
       l->tail_ = timer->pre_;
       if(l->tail_)
           l->tail_->next_ = NULL;
   }
   else
   {
       timer->pre_->next_ = timer->next_;
       timer->next_->pre_ = timer->pre_;
   }
   timer->next_ = timer->pre_ = NULL;
   timer->linked_list_ = NULL;
}
void freeTimer(Timer* t)
{
    free(t);
}
void _SwitchOthWheel(TimeWheel*tw,TimerList*l, size_t idx  )
{
    if( l[idx].head_ == NULL )
        return;
    Timer* node = l[idx].head_ ;
    Timer* temp = NULL;
    while( node  )
    {
        temp = node->next_;
        _AddTimer(tw,node);
        node = temp;
    }
    l[idx].head_ = NULL;
    l[idx].tail_ = NULL;
}
void SwitchOthWheel(TimeWheel* tw )
{
    size_t index= tw->tick_&TV1_MASK;
    if(index)
        return;
    //1级时间轮子为0 则必然发生了进位
    index = (tw->tick_>>TV1_BIT)&TV_MASK;
    //将二级时间轮上的定时器重新添加进时间轮上 则会添加到一级轮上
    _SwitchOthWheel(tw,tw->tv2_, index );
    if(index)
        return;
    //3 4 5级类似
    index = (tw->tick_>>(TV1_BIT+TV_BIT))&TV_SIZE;
    _SwitchOthWheel(tw,tw->tv3_, index );
    if(index)
        return;
    index = (tw->tick_>>(TV1_BIT+2*TV_BIT))&TV_SIZE;
    _SwitchOthWheel(tw,tw->tv4_, index );
    if(index)
        return;
    index = (tw->tick_>>(TV1_BIT+3*TV_BIT))&TV_SIZE;
    _SwitchOthWheel(tw,tw->tv5_, index );
    return;
}
void Tick(TimeWheel* tw )
{
    uint64_t now = now_milliseconds(); 
    if( tw->tick_>now )
        return;
    while( tw->tick_<=now  )
    {
        tw->tick_++;
        //除了 tv1 其他轮子上面的timer不容易触发
        SwitchOthWheel(tw);
        size_t idx = tw->tick_ & TV1_MASK;
        if(tw->tv1_[idx].head_ !=NULL )
        {
            Timer* node = tw->tv1_[idx].head_;
            while( node !=NULL   )
            {
                Timer* temp = node->next_;
                node->handle_(node->data_);
                if( node->timer_options_.flag_ == 1 ||
                    (node->timer_options_.flag_ ==2 && --node->timer_options_.execu_times_ !=0  ))
                {
                    node->expried_time_=node->timer_options_.timeout_+tw->tick_;
                    _AddTimer(tw,node);
                }
                else
                    freeTimer(node);
                node = temp;
            }
            tw->tv1_[idx].head_=NULL;
        }
    }
}
#ifdef __TimeWheel_Test_
void display_time(const char*msg)
{
    time_t  now = now_milliseconds();
    struct  tm  now_tm;
    time_t  sec_now = now/1000;
    localtime_r(&sec_now, &now_tm);
    char    time_buf[64]={0};
    snprintf( time_buf, sizeof(time_buf), "%04d-%02d-%02d %02d:%02d:%02d %03ld", 
            now_tm.tm_year+1900,now_tm.tm_mon+1,now_tm.tm_mday,now_tm.tm_hour,now_tm.tm_min,now_tm.tm_sec,now%1000);
    fprintf( stderr,"%s  %s\n", msg, time_buf );
}
void* time_out(void* args )
{
    fprintf( stderr, " %ld ms超时的处理 ", (size_t)args );
    display_time("超时");
}
#include <sys/select.h>
#include <errno.h>
void Test()
{
    TimeWheel* wh = InitTimeWheel();
    Timer* t1 = AddTimer(wh,time_out,50,(void*)50,Exec_Once,0);
    Timer* t4 = AddTimer(wh,time_out,1000,(void*)1000,Exec_Times,3);
    Timer* t5 = AddTimer(wh,time_out,5000,(void*)5000,Exec_All,0);
    while(1)
    {
        //select 每次调用返回 tv都会改变 下次调用要重新赋值
        struct timeval tv3={0,1000};
        //display_time("Tick ");
        int ret = select(0,NULL,NULL,NULL,&tv3);
        Tick(wh);
    }
}
int main()
{
    Test();
}
#endif
