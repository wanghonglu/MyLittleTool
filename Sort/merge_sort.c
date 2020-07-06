#include "merge_sort.h"
//迭代版归并
void merge_sort(int *ptr, int len )
{
    if( NULL == ptr || len == 0 )
        return;
    int *result  = (int*)malloc(len*sizeof(int));
    assert(NULL !=result );
    _merge_sort(ptr, result, len );
    memcpy( ptr, result, len*sizeof(int));
    free(result);
}
#define Min( x, y ) (x)<(y)?(x):(y)
void _merge_sort(int*ptr, int*result, int len )
{
    int seg = 1;//分块大小
    int start =0;

    int *a = ptr;

    for( ;seg<len;seg *=2 )//最外层的循环控制每次切分大小
    {
        for(start =0;start<len; start=start+2*seg )
        {
            int left = start;
            int k = start;
            //  这里一定要跟 len-1 对比取一个最小的 防止溢出
            int right = Min(start+seg, len-1 );
            int left_end = Min(right-1, len-1);
            int right_end = Min( start+2*seg-1, len-1 );
            while( left<= left_end && right<=right_end )
            {
                if( a[left]<a[right] )
                    result[k++]=a[left++];
                else
                    result[k++]=a[right++];
            }
            //剩余的数据归并到集合里
            while(left<=left_end )
                result[k++]=a[left++];
            while(right<=right_end)
                result[k++]=a[right++];
        }
        int *tmp = result;
        result  = a;
        a = tmp; 
    }

    if( a == ptr )
        memcpy( result, a,len*sizeof(int)); 

}
//递归版的归并排序
void 
recursive_merge_sort(int* ptr,int len )
{
    assert( NULL != ptr && len >0 );
    int *result = (int*)malloc(len*sizeof(int));
    assert( NULL != result );

    _recursive_merge_sort(ptr, result, 0, len-1 );

    memcpy(ptr, result, sizeof(int)*len);
    free(result );
}
void
_recursive_merge_sort(int *ptr, int*result, int beg, int end )
{
    if( beg>=end )
        return;
    int mid = beg + ( end-beg)/2;

    _recursive_merge_sort(ptr, result, beg, mid );
    _recursive_merge_sort(ptr, result, mid+1, end );

    int start1=beg, start2 = mid+1;
    int k = beg;
    while( start1<=mid && start2<=end )
    {
        if( ptr[start1]<ptr[start2])
            result[k++]=ptr[start1++];
        else
            result[k++]=ptr[start2++];
    }
    //归并剩余数据
    while( start1<=mid )
        result[k++]=ptr[start1++];

    while( start2<=end )
        result[k++]=ptr[start2++];

    //需要把排序好的部分 拷贝到原数组
    memcpy( ptr+beg,result+beg,  (end-beg+1)*sizeof(int ));


}


