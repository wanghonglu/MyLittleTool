#include "quick_sort.h"
void 
quick_sort(int*ptr, int len)
{
    assert( NULL!=ptr && len>0 );
    _quick_sort(ptr, 0, len-1 );
    return;
}
//这么写其实会有问题 如果两个数是同一个,这里会把这个数改成0,值相同也可以,但不能是同一个
//Swap(ptr[left],ptr[right]) 如果left==right 这里最终值会改成1 所以还是用中间变量替换吧
//就是如果是 a=5,b=5这种可以Swap(a,b) 但是Swap(a,a)就会出错 最终a=0
#if 0
#define Swap(l,r) do{\
    l = l^r;\
    r = l^r;\
    l = l^r;\
}while(0)
#endif
#define Swap(l,r) do{\
    int temp=l;\
    l = r;\
    r = temp;\
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
    return _quick_sort(ptr, left, start1-1 ),_quick_sort(ptr, start1+1, right );
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
    //printf("%d\n", privote );
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
//三数取中法
//即从left  mid right三个数中挑选一个中间的 
//然后swap到left或者right的位置,回到上面的情况
void
better_quick_sort( int*ptr, int len )
{
    assert( NULL != ptr && len>0 );
    _better_quick_sort( ptr, 0, len-1 );
} 
//三个数中选择中间那个数
//取中间那个数 放在right-1的位置当作基准值 这样right 位置的数比right 大 符合基准值的定义
void 
choose_right(int *ptr, int left, int right )
{
    //跟冒泡差不多的思路
    int mid = left+(right-left)/2;
    if( ptr[mid]<ptr[left] )
    {
        Swap(ptr[left], ptr[mid] );
    }

    if( ptr[right]<ptr[mid] )
    {
        Swap(ptr[right], ptr[mid] );
        if( ptr[mid]<ptr[left] )
            Swap(ptr[mid], ptr[left] );
    }

    Swap( ptr[mid], ptr[right]);//这里把选好的基准值 放在了right-1的位置 这里right已经比基准值大了 就没必要再排序了
    return;
}
void
_better_quick_sort(int *ptr, int left, int right )
{
    if( right-left<=1 )
    {
        if( (1 == right - left) && ptr[left]>ptr[right] )
            Swap( ptr[left], ptr[right] );
        return;
    }

    //三数取中 调整后 基准值在right的位置
    choose_right(ptr, left, right );
    int pivot = ptr[right];
    int start1 = left, start2 = right;
    while( start1<start2 )
    {
        //基准值在右边 空位在右边 先遍历左边
        while( start1<start2 && ptr[start1]<=pivot )
            ++start1;
        if(start1<start2)
            ptr[start2--] = ptr[start1];//空位转移到左边 
        while( start1<start2 && ptr[start2]>=pivot )
            --start2;
        if(start1<start2)
            ptr[start1++] = ptr[start2];//空位转移到右边
    }
    ptr[start1] = pivot;
    _better_quick_sort(ptr, left, start1-1);
    _better_quick_sort(ptr,start1+1, right);
}
//去重版的快排
void
better_quick_sort2(int*ptr, int len)
{
    assert( ptr!=NULL && len>0 );
    _better_quick_sort2( ptr, 0, len-1 );
    return;
}
void
_better_quick_sort2( int *ptr, int left, int right )
{
    if( right - left <=1 )
    {
        if( 1 == right-left && ptr[left]>ptr[right] )
            Swap(ptr[left], ptr[right]);
        return;
    }
    //三数选中 此时中间的数位于 right位置
    choose_right(ptr, left, right );
    int pivot = ptr[right];
    int start1 = left, start2 = right;

    while( start1<start2 )
    {
        //空位在右边
        while( start1<start2 && ptr[start1]<=pivot )
            ++start1;
        //左边有一个数大于基准值 填到空位上,空位转移到左边
        if( start1<start2 )
            ptr[start2--] = ptr[start1];
        
        while( start1<start2 && ptr[start2]>=pivot )
            --start2;
        //右边有一个值小于基准值 填到左边的空位上,空位转移到右边
        if( start1<start2 )
            ptr[start1++] = ptr[start2];
    }

    ptr[start1] = pivot;
    //这时 start1左边的值都小于等于它 右边的值都大于等于它
    // 针对其中重复的,可以挑出来放到它两边 保证这部分重复的数据不再进行排序
    int left_idx=start1-1, right_idx=start1+1;
    deal_repeat( ptr, left, start1, right, &left_idx, &right_idx );


    if( left_idx>left )
        _better_quick_sort2(ptr, left, left_idx );
    
    if( right>right_idx )
        _better_quick_sort2(ptr, right_idx, right );
}
void
deal_repeat(int *ptr, int left, int pos, int right, int*pos_left, int*pos_right )
{
    *pos_left = pos - 1;
    *pos_right = pos + 1; 


    int i=0;
    for( i=pos-1;i>=left;i-- )
    {
        if( ptr[pos] == ptr[i] )
        {
            if( *pos_left != i )
            {
                Swap( ptr[i], ptr[*pos_left] );
            }
            *pos_left -=1;
        }
    }

    for( i = pos+1;i<=right;i++ )
    {
        if( ptr[pos] == ptr[i] )
        {
            if( *pos_right != i )
            {
                Swap( ptr[i], ptr[*pos_right] );
            }
            *pos_right +=1;
        }
    }

    return;
}

//快排的尾递归优化
void 
better_quick_sort3( int *ptr, int len )
{
    assert( ptr!=NULL && len>0 );
    _better_quick_sort3( ptr, 0, len -1 );
}
//为了好看 把切分的部分单独提炼成函数
int
partition( int*ptr, int left, int right )
{
    //三数取中  会把 left  right mid 三数中的中间的数放在right字段
    choose_right(ptr, left, right );
    int pivot = ptr[right];
    int start1 = left;
    int start2 = right;

    while( start1<start2 )
    {
        while( start1<start2 && ptr[start1]<=pivot )
            ++start1;
        //左边找到合适的值 交换右边的空位 空位转移到左边
        if( start1<start2 )
            ptr[start2--] = ptr[start1];
        while(start1<start2 && ptr[start2]>=pivot )
            --start2;
        //右边的数填在左边的空位上 空位再次转移到右边
        if( start1<start2 )
            ptr[start1++] = ptr[start2];
    }
    ptr[start1] = pivot;
    return start1;
}
void
_better_quick_sort3( int *ptr, int left, int right )
{

#if 0
    if( right -left <=1 )
    {
        if( right - left == 1 && ptr[left]>ptr[right] )
            Swap( ptr[left], ptr[right] );
        return;
    }
    int partitionpos = partition(ptr, left, right );
    int left_pos, right_pos;//去重
    deal_repeat( ptr, left, partitionpos, right, &left_pos, &right_pos ); 

    if( left_pos>left )
        _better_quick_sort3(ptr, left, left_pos );
    if( right_pos<right );
        _better_quick_sort3( ptr, right_pos,right );
#endif
    //这么写跟上面相比 没有快啊 开了编译选项O2也没快 不知道为什么stl的sort为什么要这么写..
    while( left<right )
    {
        if( right -left <=1 )
        {
            if( right - left == 1 && ptr[left]>ptr[right] )
                Swap( ptr[left], ptr[right] );
            return;
        }
        int partitionpos = partition(ptr, left, right );
        int left_pos, right_pos;//去重
        deal_repeat( ptr, left, partitionpos, right, &left_pos, &right_pos ); 
    
        if( left_pos - left < right - right_pos )
        {
            _better_quick_sort3(ptr, left, left_pos );
            left = right_pos;
        }
        else
        {
            _better_quick_sort3(ptr, right_pos, right );
            right = left_pos;
        }
    }
}













