#include "radix_sort.h"
void
radix_sort_LSD(int*ptr, int len )
{
   assert( ptr && len>0 );
   
   int max = ptr[0];
   for( int i=1;i<len;i++ )
   {
       if( ptr[i] >max )
           max = ptr[i];
   }

   long  n=1;
   int count[10]={0};
   int *result = (int*)malloc(sizeof(int)*len );
   int *a = ptr;
   assert(result);
   //这个n在乘以10的过程中会溢出 比如 max本来就是INT_MAX 那么当它除 (INT_MAX/10)之后还是正的 这时候n再乘10就会溢出
   //要么让n的精度比max高要么这里判断>0
   while( max/n>0 )      
   {
       memset( count, 0x00, sizeof(count));
       for( int i=0;i<len;i++ )
            count[(a[i]/n)%10]++;//n位1 统计各位各个数的次数

       //这里累加是为了计算出每个数在结果数组的具体下标
       //比如count[0]=5 count[1]=2 那么就是说个位为1的数的下标应该是6 7 即:count[1]-1
       for( int i=1;i<10;i++ )
           count[i]+=count[i-1];

       //这里为什么要反向遍历原数组 不太清楚  先试试
       for( int i=len-1;i>=0;i-- )
       {
            result[--count[(a[i]/n)%10]] = a[i];
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
}
