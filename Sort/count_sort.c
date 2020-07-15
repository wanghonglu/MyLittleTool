#include "count_sort.h"
void 
count_sort( int*ptr, int len )
{
    if( NULL == ptr || len<=1 )
        return;
    int max = ptr[0];
    int min = ptr[0];
    int has_negtive=0;
    for( int i=1;i<len;i++ )
    {
        if( ptr[i]>max )
            max = ptr[i];
        if( ptr[i]<min)
            min = ptr[i];
    }
    
    if( min<0 )//有负数
    {
        max -=min;
        for( int i=0;i<len;i++ )
            ptr[i]-=min;
        has_negtive = min;
        min =0;
    }

    if( max-min>10000 )//计数排序不适用与数据特别分散的排序
    {
        fprintf( stderr, "数据太分散不适用于计数排序\n" );
        return;
    }

    int countLength = max-min+1;
    int* count = (int*)malloc(sizeof(int)*countLength );
    assert( count !=NULL );
    memset(count, 0x00, sizeof(int)*countLength ); 

    for( int i=0;i<len;i++ )
        count[ptr[i]-min]++;

    //类似基数数组的处理 将计数数组的内容转化成数组小标 比如c[0]=2 c[1]=3 则认为为0的数据数组下标分别是0 1 为1的数据数组下标为4 3 2 
    // 同样的如果想要从大到小排序
    // ,这里也要相应的变成 for(int i=countLength-2;i>=0;i--)count[i]+=count[i+1];
    for( int i=1;i<countLength;i++ )
        count[i]+=count[i-1];

    int *result = (int*)malloc(sizeof(int)*len);
    assert( NULL!=result );

    //之所以要倒叙是同样的数字,先统计到的是0 后统计到的变成1 ,而遍历数组的时候 先遍历的大的
    //即会先往后放 再往前放 比如上面为1的数组 第一个放进去的在4 后面的在3 2 跟统计时候一样
    for( int i=len-1;i>=0;i-- )
        result[--count[ptr[i]-min]] = ptr[i];

    memcpy( ptr, result, sizeof(int)*len );
    free( result );
    free(count);

    if( has_negtive != 0 )
    {
        for( int i=0;i<len;i++ )
            ptr[i]+=has_negtive;
    }
    
    return;
}
