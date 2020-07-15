#include "radix_sort.h"
//基数
#define RadixSize 10
#define Swap(a,b ) do{\
    int tmp = a;    \
    a =b;           \
    b = tmp;        \
}while(0)
void
radix_sort_LSD(int*ptr, int len )
{
    if( ptr == NULL || len<=1 )
        return;
   
   int max = ptr[0];
   int min = ptr[0];//处理负数的情况,找出最小数,判断是否为负数,然后统一给原数组减去这个负数,保证最小数变成0
   for( int i=1;i<len;i++ )
   {
       if( ptr[i] >max )
           max = ptr[i];
       if( ptr[i] <min )
           min = ptr[i];
   }

   if( min<0 )
   {
       max -=min;
       for( int i=0;i<len;i++ )
           ptr[i]-=min;
   }

   long  n=1;
   int count[RadixSize]={0};
   int *result = (int*)malloc(sizeof(int)*len );
   int *a = ptr;
   assert(result);
   //这个n在乘以10的过程中会溢出 比如 max本来就是INT_MAX 那么当它除 (INT_MAX/10)之后还是正的 这时候n再乘10就会溢出
   //要么让n的精度比max高要么这里判断>0
   while( max/n>0 )      
   {
       memset( count, 0x00, sizeof(count));
       for( int i=0;i<len;i++ )
            count[(a[i]/n)%RadixSize]++;//n位1 统计各位各个数的次数

       //这里累加是为了计算出每个数在结果数组的具体下标
       //比如count[0]=5 count[1]=2 那么就是说个位为1的数的下标应该是6 7
       //所以需要count[1]+=count[0]  即到时候往数组里面安排的时候就是:count[1]-1
       for( int i=1;i<RadixSize;i++ )
          count[i]+=count[i-1];

       /*
        *   如果是从大到小排序 这里就应该是倒着安排
        for( int i=RadixSize-2;i>=0;i-- )
           count[i] +=count[i+1];//即安排个位最大的放在最前,后面的都不需要改 注意这里的i条件是>=0
        * */

       //这里为什么要反向遍历原数组?
       //上一次数组里面的数据 假如此时的是十位 则上一次放进数组的值是个位的
       //即从头往后是个位的从小到大的数据
       //而count[1]=7 代表十位为1的数是要放在 6 5两个位置上, 先放的6 后放的5 所以6位置先放一个个位较大的,5位置放一个个位较小的,这样就能保证整个数组从小到达排序
       for( int i=len-1;i>=0;i-- )
       {
            result[--count[(a[i]/n)%RadixSize]] = a[i];
       }
       //用result数组覆盖原数组
       int *tmp = a;
       a = result;
       result  = tmp;
       n*=10;
   }
   if( a !=ptr )
   {
       memcpy( ptr, a, sizeof(int)*len );
       result = a;  
   }
   free(result);
   if( min<0 )
   {
       for( int i=0;i<len;i++ )
           ptr[i]+=min;
   }
}

//MSG (most significandt digital ) 基数排序的MSG 从左边关键字开始
void
radix_sort_MSD(int*ptr, int len )
{
    if( NULL == ptr || len<=1 )
        return;    
    int min = ptr[0],max=ptr[0];
    for( int i=1;i<len;i++ )
    {
        if( ptr[i]>max )
            max = ptr[i];
        if( ptr[i]<min )
            min = ptr[i];
    }
    if( min<0 )//处理负数的情况
    {
        max -=min;
        for( int i=0;i<len;i++ )
            ptr[i]-=min;
    }
    //计算最大值的最左边的有效位的整数
    int n=1;
    while( max = max/10 )//这里不能做成最后n/=10 因为如果max足够大的话 n会溢出
       n*=10; 
    _radix_sort_MSD_recursive(ptr, 0, len-1, n );

    if( min<0 )//处理负数的情况
    {
        for( int i=0;i<len;i++ )
            ptr[i]+=min;
    }
}
void
_radix_sort_MSD_recursive(int*ptr, int start, int end, int exp )
{
    if( exp<=0 )
        return;//最后的个位虽然桶里由多个数据但是已经有序不需要再排了
    if( end - start<=1 )
    {
        if( 1 == end-start && ptr[start]>ptr[end] )
            Swap( ptr[start],ptr[end] );
        return;
    }
    int* bucket = (int*)malloc(sizeof(int)*(end-start+1));
    assert( NULL != bucket );

    int count[RadixSize]={0};

    //此时count数组里是每个高为为0-9的数量 也可能是0-99 取决于RadixSize
    for( int i=start;i<=end;i++ )
        count[ (ptr[i]/exp)%RadixSize ]++;

    //计算出该桶内数据真正该存放的地址
    //如果是倒序 这里应该是倒着安排 i=RadixSize-2;i>0;i--
    //count[0]=1 count[1]=2 则0号桶里的元素在0号位置 1号桶的元素分别在2号为 和1号位置
    for( int i=1;i<RadixSize;++i )
        count[i]+=count[i-1];

    //放入桶内 这里倒叙是因为上一步ptr数组里面 是按照前一位的从小到大排序的
    //这里针对当前一位相同的情况 优先把上一位大的元素安排在更后的位置
    for( int i=end;i>=start;i-- )
    {
        bucket[--count[(ptr[i]/exp)%RadixSize]]=ptr[i];
    }
    //排序好的部分拷贝到原数组中
    memcpy( ptr+start, bucket,sizeof(int)*(end-start+1) );

    for( int i=0;i<RadixSize;i++ )
    {
        int start1 = start+count[i];
        //i的右边界 count[i+i]-1 !!!
        int end1 = (i+1)<RadixSize?(start + count[i+1]-1):end;
        if( start1<end1 )
        {
            //递归 再次在子结构中排序
            _radix_sort_MSD_recursive(ptr, start1, end1, exp/RadixSize );
        }
    }
    free( bucket );

}
#undef Swap







