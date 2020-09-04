#include<stdio.h>
#include "coroutine_ctx.h"
#include<unistd.h>
#define Test_COUNTS 10000
void fun(void* args )
{
    printf("fun-->%ld\n", (size_t)args );
}
void fun1(void* args )
{
    int a = (size_t)args;
    size_t i=0;
    common_shared_stack_t* shared_st = create_shared_stack(0);

    context_t** co_array;
    co_array = (context_t**)malloc(Test_COUNTS*sizeof(void*));
    for( ;i<Test_COUNTS;i++  )
    {
       co_array[i] = co_create_shared_stack(fun, (void*)i,shared_st ); 
    }
    printf("create %d 个协程 end \n", Test_COUNTS );
    sleep(30);
    for( i=0;i<Test_COUNTS;i++  )
    {
       co_start(co_array[i]);
       clear_stack(co_array[i]);
    }
    printf("all co end %ld\n", i+a );
    shared_stack_free(shared_st);
    free(co_array);
}
int main()
{
    size_t count=100;
    common_shared_stack_t* shared_st = create_shared_stack(0);
    context_t* co=co_create_shared_stack(fun1,(void*)count, shared_st  );
    co_start(co);
    clear_stack(co);
    shared_stack_free(shared_st);
}

