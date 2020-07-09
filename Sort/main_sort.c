#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/time.h>
#include<stdlib.h>

#include "merge_sort.h"
#include "quick_sort.h"
#include "insert_sort.h"
#include "bubble_sort.h"
#include "select_sort.h"
#include "heap_sort.h"
#include "radix_sort.h"
/*
 *  十大排序算法
 *  https://www.runoob.com/w3cnote/selection-sort.html
 * */
//产生随机数组
int *ProduceRandomArray(int len)
{
    static int init=0;
    if(!init)
    {
        srand(time(NULL));
        init = 1;
    }
    int *ptr = (int*)malloc(sizeof(int)*len);
    assert(ptr != NULL );
    int i=0;
    for(;i<len;++i)
        ptr[i]=random();
    return ptr;
}
long
now_mic()
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return tm.tv_sec*1e6+tm.tv_usec;    
}
typedef void(*fun)(int*,int );
typedef struct SortTest{
    char   m_msg[1024];         //排序算法名称
    fun    m_sort_fun;    //排序算法实现
    double m_cost_millseconds;//排序算法耗时
    int    m_right;
}SortTest;

void PrintArray(const char*msg,  int* ptr, int len )
{
    if( len>100 )
        return;
    int i=0;
    printf( "%s\n", msg );
    for( i=0;i<len;i++ )
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}
/*
 *  抽样检查,首先会做一次全量检查看数组是否有序,为了防止看似有序实则数据错乱 比如夹杂0进去
 *  做抽样检查
 * */
static int has_check_number=0;
#define CheckCounts 50
static int checkIdx[CheckCounts]={0};
static int checkNumber[CheckCounts];
int Check(int *ptr, int len )
{
    if(len <=1 )return 1;
    static int middle_int =-1;

    int idx=0;
    for( idx=1;idx<len;idx++ )
    {
        if( ptr[idx]<ptr[idx-1] )
            return 0;
    }
    if( !has_check_number )
    {
        int counts = len>CheckCounts?CheckCounts:len/2;
        for( int i=0;i<counts;i++ )
        {
            checkIdx[i] = random()%len;//存抽样检查的数组下标 随机抽取抽样数组下标
            checkNumber[i]=ptr[checkIdx[i]];//存第一次排序 抽样下标的具体值
        }
        has_check_number = 1;
    }
    else
    {
        for( int i=0;i<CheckCounts;i++ )
        {
            if( checkIdx[i]!=0 &&  ptr[checkIdx[i]] != checkNumber[i] )
            {
                return 0;
            }
        }
    }
    return 1;
}
#define IntLen(a) sizeof(a)/sizeof(int)
#define MaxOrderredNumber 100000 
int *orderredNumber = NULL;//有序数组
#define MutipleNumberArray 100000
int *mutipleNumberArray = NULL;//全部重复数组
//产生有序数组
void GetOrderredNumber()
{
    if( orderredNumber )
        free(orderredNumber);
    orderredNumber = (int*)malloc(sizeof(int)*MaxOrderredNumber);
    int beg = 1;
    int i=0;
    for( ;i<MaxOrderredNumber;i++ )
        orderredNumber[i]=beg+i;

    if( MaxOrderredNumber>100 )
    {
        for(;i<50;i++ )
        {
            int idx = random()%MaxOrderredNumber;
            orderredNumber[idx] = random();
        }
    }

}
//产生全部相同数组
void GetMutipleNumberArray()
{
    if( mutipleNumberArray )
        free(mutipleNumberArray);
    mutipleNumberArray =(int*)malloc(sizeof(int)*MutipleNumberArray);
    int beg = random();
    int i=0;
    for( ;i<MutipleNumberArray;i++ )
        mutipleNumberArray[i]=beg;

    if( MutipleNumberArray>100 )
    {
        for(;i<50;i++ )
        {
            int idx = random()%MutipleNumberArray;
            mutipleNumberArray[idx] = random();
        }
    }
}
int cmp( const void* r, const void* l )
{
    return *(int*)r-*(int*)l;
}
int TestCmp(const void*r, const void*l )
{
    return ((SortTest*)r)->m_cost_millseconds -  ((SortTest*)l)->m_cost_millseconds;
}
void std_quick_sort_SortTest( SortTest*ptr, int len )
{
    return qsort( ptr, len, sizeof(SortTest ), TestCmp );
}
void std_quick_sort(int*ptr, int len )
{
    return qsort( ptr, len, sizeof(int), cmp); 
}
int main(int argc ,char**argv)
{
    GetOrderredNumber();
    GetMutipleNumberArray();
    int count =0;
    if( argc >= 2 )
        count  = atoi(argv[1]);
    int sort_idx=0;
    SortTest arry[]={
        {"C标准库的qsort", std_quick_sort},
        {"迭代版归并排序", merge_sort },
        {"递归版归并排序", recursive_merge_sort},
        {"普通快排", quick_sort},
        {"基准选右边快排", quick_sort2},
        {"三数取中法快排", better_quick_sort},//数组如果本来就有序 复杂度将达到n^2
        {"带去重的三数取中法快排", better_quick_sort2},//数组如果本来就有序 复杂度将达到n^2
        {"尾递归优化的快排", better_quick_sort3},//数组如果本来就有序 复杂度将达到n^2
        //{"插入排序", insert_sort},
        {"希尔折半增量排序", shell_insert_sort},
        {"希尔质数增量排序", shell_insert_sort_partitionbyprime},
        //{"冒泡排序", bubble_sort},
        //{"选择排序", select_sort},
        {"堆排序", heap_sort},
        {"基数排序LSD", radix_sort_LSD},
        {"", NULL}
    };
    int a[]={3,3,1,3,3,199,90,0,10000,892,3,2,3,3,3,7,0};

    int *ptr = NULL;
    if(count > 0 )
    {
        ptr = ProduceRandomArray(count);
    }
    else if( count == -1 )//有序数据排序
    {
        ptr = orderredNumber;    
        count = MaxOrderredNumber;
    }
    else if( count == -2 )//重复数据排序
    {
        ptr =mutipleNumberArray;
        count = MutipleNumberArray;  
    }
    else 
    {
        ptr = a;
        count = sizeof(a)/sizeof(int);
    }

    int *b = (int*)malloc(sizeof(int)*count);
    assert( NULL!=b);

    memcpy( b, ptr,count*sizeof(int));
    SortTest *tmp=NULL;
    while( (tmp = &arry[sort_idx++]) && tmp->m_sort_fun !=NULL )
    {
        memcpy( ptr, b, count*sizeof(int));
        if( ptr == a )
        {
            printf("========\n");
            printf("%s ", tmp->m_msg );
            PrintArray( "排序前", ptr, count );
        }
        long t1 = now_mic();
        tmp->m_sort_fun( ptr, count );
        if( ptr == a )
        {
            PrintArray( "排序后", ptr, count );
        }
        tmp->m_cost_millseconds = (double)(now_mic()-t1)/1000;
        tmp->m_right = Check(ptr, count);
        printf("%s end\n", tmp->m_msg);
    }
    std_quick_sort_SortTest(arry, sizeof(arry)/sizeof(SortTest) - 1 );
    sort_idx=0;
    while( (tmp = &arry[sort_idx++]) && tmp->m_sort_fun !=NULL )
    {
        fprintf( stdout, "%-20s%-10s 耗时:%.2lf毫秒\n", tmp->m_msg, tmp->m_right?"":"错误!!!!!!!", tmp->m_cost_millseconds );
    }
    fprintf( stderr, " 待排序数组个数:%d,待排序数组为:%s\n", count, 
    ptr == a?" 给定数组 ":ptr==orderredNumber?" 绝大部分有序数组 ":ptr == mutipleNumberArray?" 绝大部分重复数组":" 随机产生数组" );
    
    if( ptr!=a )
        free(ptr);
    free(b);
}














