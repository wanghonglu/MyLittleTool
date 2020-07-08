#include "insert_sort.h"
void
insert_sort( int*ptr, int len )
{
    assert( NULL !=ptr && len>0 );

    int i,j;
    for( i=1;i<len;i++ )
    {
        if( ptr[i]<ptr[i-1] )
        {
            int temp = ptr[i];
            for(j=i;j>0 && ptr[j-1]>temp ;j-- )
                ptr[j] = ptr[j-1];

            ptr[j] = temp;
        }
    }
}
/* 希尔排序  
 *   折半增量  分组增量按照 n/2 n/4 .....
 *   4 3 2 5 8 10 1
 * */
void
shell_insert_sort( int *ptr, int len )
{
    assert( ptr && len>0 );
    int i,j,k;
    for( i=len/2;i>0;i=i/2 )
    {
        //这里是j++ 而不是j+i 因为2分之后 相当于 a[9]和a[2]比较
        for( j=i;j<len;j++ )
        {
            int temp = ptr[j];
            for( k = j;k>=i && ptr[k-i]>temp;k-=i )
               ptr[k] = ptr[k-i];
            ptr[k] = temp;
        }
    }
}
/*  希尔排序质数分割
 *  2^k-1  1 3 7 15 31 ...
 *  首先通过这个公式找到下雨len的最大质数 然后按照 (k-1)/2算出下一次分组大小
 * */
void
shell_insert_sort_partitionbyprime( int *ptr, int len )
{
    assert( ptr && len >0 );

    int maxPrime =1,idx=1;
    for( ;pow(2,idx)-1<len;idx++ )
        ;
   maxPrime = pow(2,idx-1)-1; 
   int i,j,k;
    for( i=maxPrime;i>0;i = (i-1)/2 )
    {
        //printf("prime:%d\n", i );
        for(  j=i; j<len;j++ )
        {
            int temp = ptr[j];
            for( k=j;k>=i && ptr[k-i]>temp ;k-=i )
               ptr[k] = ptr[k-i];

            ptr[k] = temp;
        }
    }
}






