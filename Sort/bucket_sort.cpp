#include "bucket_sort.h"
void 
bucket_sort_quick( int*ptr, int len )
{
    if( NULL == ptr || len<=1 )
        return;
    int min=ptr[0];
    int max=ptr[0];
    for( int i=0;i<len;++i )
    {
        if( ptr[i]<min )
            min = ptr[i];
        if( ptr[i]>max )
            max = ptr[i];
    }
    int bucketsize = max/10-min/10+1;

    std::vector<std::vector<int>> bucketarray(bucketsize);
    //入桶
    for( int i=0;i<len;++i )
    {
        int bucket = (ptr[i]/10 - min/10)%bucketsize;
        bucketarray[bucket].push_back(ptr[i]);
    }
    //分别对每个桶排序  
    //分别对每个桶进行排序 并将结果放入原数组中 
    int index=0;
    for( int i=0;i<bucketarray.size();++i )
    {
        //空桶直接跳过
        if( bucketarray[i].size() == 0 )
            continue;
        //这里用快排
        better_quick_sort2( bucketarray[i].data(), bucketarray[i].size() ); 
        //排序完成 装回原来的数组
        for(int j=0;j<bucketarray[i].size();j++ )
            ptr[index++]=bucketarray[i][j];
   }
}
