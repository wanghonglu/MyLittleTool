#include"coroutine_ctx.h"
#include<pthread.h>
#include<stdio.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<unistd.h>
pthread_cond_t   cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t  mtx = PTHREAD_MUTEX_INITIALIZER;

context_t ctx_main;
context_t ctx_thread;
context_t ctx_fun;
int flag=0;
__thread int aa=0;
void fun(void* args ,void*args2,void* args3)
{
    size_t  tid1 = syscall(SYS_gettid);
    jump_2_context(&ctx_fun,&ctx_thread);
    size_t  tid2 = syscall(SYS_gettid);
    printf("tid1=%ld == tid2=%ld\n", tid1,tid2 );
    jump_2_context(&ctx_fun,&ctx_main);
}
void* thread_fun(void* args )
{
    jump_2_context(&ctx_thread, &ctx_fun);
    ++aa;

    pthread_mutex_lock(&mtx);
    flag = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
    return NULL;
}
int main()
{
    make_stack(&ctx_fun,fun,NULL,NULL,NULL);

    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mtx,NULL);


    pthread_t  thread;
    pthread_create(&thread, NULL, thread_fun, NULL );

    pthread_mutex_lock(&mtx);
    while( flag == 0  )
    {
        pthread_cond_wait(&cond,&mtx);
    }
    pthread_mutex_unlock(&mtx);
    jump_2_context(&ctx_main,&ctx_fun);


    pthread_join(thread,NULL);

    printf("aa==%d\n", aa );

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

    clear_stack(&ctx_fun);

}
