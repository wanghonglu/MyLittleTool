#ifndef _BUCKET_SORT__H
#define _BUCKET_SORT__H
#ifdef __cplusplus
extern "C"{
#endif
/*
 *  桶排序:
 *          桶排序其实是对计数排序的改进,计数排序 适合数组差异不大,并且紧密的数列
 *          如果数列就三个值 1 2 1000 先然用1000个大小的数组来排序并不合适
 *          桶排序就是按照一定的规则将数据分开成不同的桶,然后每个桶再调用别的排序算法
 *          比如比较排序算法 快排 堆排序 或者归并之类的算法
 *          是个灵活的排序算法
 * */
void
bucket_sort( int *ptr, int len );
#ifdef __cplusplus
}
#endif
#endif
