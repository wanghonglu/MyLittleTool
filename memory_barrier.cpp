#include<iostream>
#include<thread>
#include<chrono>
#include<mutex>
#include<cstring>
//多线程间可共享的信号量,提供原子操作
#include<semaphore.h>
#include<cstdlib>
#include<ctime>
using namespace std;
int a=0;
int b=0;
int x=0;
int y=0;
sem_t  sem_thread1;
sem_t  sem_thread2;
sem_t  sem_end;
int main(int argc,char**argv)
{
    /*
     * 这台机器白搭  这台机器是单核的 草~~~~~
     * */
    srand(time(0));
    sem_init(&sem_thread1, 0, 0 );
    sem_init(&sem_thread2,0,0);
    sem_init(&sem_end,0,0 );
    std::thread th1([](){
        for(;;)
        {
            //sem_thread1 >0 则-1 立马返回 否则 等待直到sem_thread>0
            sem_wait(&sem_thread1);
            while( rand()%8!=0 )
                ;
            a =2;
            x=b;

            sem_post(&sem_end);
        }
        });
    std::thread th2([](){
        for(;;)
        {
            sem_wait(&sem_thread2);
            while( rand()%8!=0 )
                ;

            b =1;
            y=a;

            sem_post(&sem_end);
        }
        });
    int count=10000;
    if( argc>1 )
        count = atoi(argv[1]);
    for ( int i=0;i<count;i++ )
    {
        a=0;
        b=0;
        //唤醒线程 sem_post 原子的将值+1
        sem_post(&sem_thread1);
        sem_post(&sem_thread2);
        //等待线程运行结束
        sem_wait(&sem_end);
        sem_wait(&sem_end);
        std::cerr<<x<<" "<<y<<std::endl;
        if( x==y && x==0 )
            std::cerr<<"x:"<<x<<" y:"<<y<<std::endl;
    }
    th1.detach();
    th2.detach();
    sem_destroy(&sem_thread1);
    sem_destroy(&sem_thread2);
    sem_destroy(&sem_end);
    return 0;
}
