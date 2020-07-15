#ifndef _BUCKET_SORT__H
#define _BUCKET_SORT__H
//stl的头文件 一定要在extern "C"的外面
//而且 因为我们的main 函数是c语言的 最终是用gcc链接的 这个时候因为有cpp 所以一定要加上 -lstdc++
// 或者链接的时候直接用g++ 
#include<vector>
#ifdef __cplusplus
extern "C"{
#endif
#include "merge_sort.h"
#include "quick_sort.h"
#include "heap_sort.h"
/*
 *  桶排序:
 *          桶排序其实是对计数排序的改进,计数排序 适合数组差异不大,并且紧密的数列
 *          如果数列就三个值 1 2 1000 先然用1000个大小的数组来排序并不合适
 *          再比如如果给定的是1-10的十个double型数组，显然 这里用计算排序不行
 *          创建11个数组 可以3.14 3.15这种数据是没法具体算出来的
 *
 *          桶排序就是按照一定的规则将数据分开成不同的桶,然后每个桶再调用别的排序算法
 *          比如比较排序算法 快排 堆排序 或者归并之类的算法
 *          是个灵活的排序算法
 *
 *          桶排序的分桶算法有很多 这里只举一种 f(n)=x/10 - min/10;
 *
 *          这里有限制 max和min之间不能相差太多
 *
 *          这里稍微改改,用归并/快排/堆排序三种进行排序
 *          桶排序适用于元素值分布较为集中的序列
 *          因为这里要建立桶 然后每个桶又要放不同的元素，所以这里用c++的vector比较方便
 *          如果用c 得事先统计出每个桶容纳的元素 再malloc 挺麻烦的
 *
 *          运行几次发现 桶排序适用于 数据比较集中的情况 如果很分散 效果不是很好
 * */
void
bucket_sort_quick( int *ptr, int len );
void
bucket_sort_merge( int *ptr, int len );
void
bucket_sort_heap( int *ptr, int len );
#ifdef __cplusplus
}
#endif
#endif
