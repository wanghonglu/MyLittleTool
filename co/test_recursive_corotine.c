#include"coroutine_ctx.h"
#include<pthread.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<unistd.h>

pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int flag =0;
context_t* funtion_co;
void mutiple_thread_co(void*args)
{
    size_t  tid1 = syscall(SYS_gettid);
    log_print("threadId tid1=%ld", tid1);

    co_yield();

    size_t  tid2 = syscall(SYS_gettid);
    log_print("threadId tid1=%ld", tid2);

}
void* thread_fun(void*args )
{
    co_start(funtion_co);
    pthread_mutex_lock(&mutex);
    flag = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void fun(void* argc )
{
    log_print("2");
}
void fun2(void* argc)
{
    log_print("1");
    context_t* co = co_create(fun, NULL);
    co_start(co);
    log_print("3");
    clear_stack(co);
}
int main()
{
    context_t* co = co_create(fun2, NULL);
    co_start(co);
    log_print("4");
    clear_stack(co);


    funtion_co = co_create(mutiple_thread_co, NULL);

    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);

    pthread_t  th;
    pthread_create(&th, NULL,thread_fun, NULL);

    pthread_mutex_lock(&mutex);
    while( flag == 0 )
    {
        pthread_cond_wait(&cond,&mutex);
    }
    pthread_mutex_unlock(&mutex);

    co_start(funtion_co);//再次启动协程
    pthread_join(th,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    log_print("all end ");
    clear_stack(funtion_co);
    return 0;
}
