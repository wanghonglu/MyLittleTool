#include "heap_sort.h"
#define Swap( a,b ) do{\
    int temp = a;\
    a = b;\
    b = temp;\
}while(0)
void
heap_sort( int*ptr, int len )
{
    if( NULL == ptr || len<=0 )
        return;
    //第一个有子节点的父节点 最后一个子节点坐标为len-1 那么其父节点左边((len-1)-1)/2
    for( int i=len/2-1;i>=0;i-- )
        heap_shift_down(ptr, len, i );
    //上面 将数组堆化,堆顶即最大 下面堆排序
    for( int i=len-1;i>0;i-- )
    {
        Swap( ptr[i], ptr[0] );//将堆顶和最后的元素互换,此时继续调整堆,但是此时堆的长度变小了
        heap_shift_down(ptr, i, 0 );
    }
}
void
heap_shift_down( int *ptr, int len, int pos  )
{
   int parent = pos;
   int son = 2*parent+1;//左孩子
   while( son<len )
   {
       //左右两孩子中挑选出最大的
       if( son+1<len && ptr[son+1]>ptr[son] )
           son+=1;//选择右孩子
        if( ptr[parent] >ptr[son] )
            break;
        Swap( ptr[parent], ptr[son] );
        parent = son;
        son = 2*parent+1;
   }
}
#undef Swap
