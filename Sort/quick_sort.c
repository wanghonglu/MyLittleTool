#include "quick_sort.h"
void 
quick_sort(int*ptr, int len)
{
    assert( NULL!=ptr && len>0 );
    _quick_sort(ptr, 0, len-1 );
    return;
}
#define Swap(l,r) do{\
    l = l^r;\
    r = l^r;\
    l = l^r;\
}while(0)
void 
_quick_sort( int*ptr, int left, int right )
{
    if( right - left<=1 )
    {
        if(1 == right - left && ptr[left]>ptr[right] )
            Swap(ptr[left],ptr[right]);
        return;
    }

    //取左边第一个值作为基准值
    int pivote = ptr[left];
    int start1 = left,start2=right;

    while( start1<start2 )
    {
        //一开始左边有空位 空位就是left
        while( start1<start2 && ptr[start2]>=pivote)
            start2--;
        //右边找到合适的数 填到左边的空位上 左边++ 右边开始有空位
        if( start1<start2 )
            ptr[start1++]=ptr[start2];

        //遍历左边 有符合条件的就放在 右边的空位上 
        while(start1<start2 && ptr[start1]<=pivote)
            start1++;
       //填入右边空位 左边开始继续出现空位 
        if( start1<start2 )
            ptr[start2--] = ptr[start1];
    }
    ptr[start1]=pivote;
    _quick_sort(ptr, left, start1-1 );
    _quick_sort(ptr, start1+1, right );
}
//快排取右边
void
quick_sort2(int*ptr, int len )
{
    assert( NULL!=ptr && len >0 );
    _quick_sort2(ptr, 0, len-1);
}
void
_quick_sort2(int*ptr, int left, int right )
{
    if( right-left<=1 )
    {
        if( right-left == 1 && ptr[left]>ptr[right])
            Swap(ptr[left], ptr[right]);
        return;
    }

    int privote = ptr[right];
    int start1 = left, start2=right;
    //基准值在右边 空位在右边 先找左边
    while(start1<start2)
    {
        while(start1<start2 && ptr[start1]<=privote )
            start1++;
        
        //空位在右边 用左边的值替换右边的空位 空位转移到左边
        if( start1<start2 )
            ptr[start2--] = ptr[start1];

        while( start1<start2 && ptr[start2]>=privote )
            start2--;

        //空位在左边 用右边的值替换左边的空位 空位转移到右边
        if( start1<start2 )
            ptr[start1++] = ptr[start2];
    }
    // 这里是不是用start1 start2都一样
   ptr[start2] = privote;
   
   _quick_sort2(ptr, left, start2-1 );
   _quick_sort2(ptr, start2+1, right );
   return;
}

