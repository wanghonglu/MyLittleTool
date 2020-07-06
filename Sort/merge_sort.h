/*
 *  归并排序：
 *  时间复杂度稳定的排序，但是需要空间复杂度O(n)
 *  c++ stable_sort就是采用归并排序
 *
 *  归并排序有两种做法，
 *  一种是从上而下的采用递归的方式
 *  另一种是从下而上的采用迭代的方式   因为递归的会额外占用栈 所以迭代的更好一点
 *
 *  两种思路都差不多，就是将待排序队列 2等分 然后再2等分 知道待排序队列剩1个元素 然后再归并
 *  归并的复杂度是n 划分位logN 所以整体的复杂度是 nlogn
 *
 * */
#ifndef __MERGE_SORT__
#define __MERGE_SORT__
#ifdef __cplusplus
extern "C"{
#endif
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<stdint.h>
#include<assert.h>
/*  迭代版的归并排序
 *
 * */
void 
merge_sort( int*ptr, int len );
void
_merge_sort( int*arry, int*result, int len );
/*递归版*/
void
recursive_merge_sort(int*ptr, int len );
void
_recursive_merge_sort(int*ptr, int*result, int beg, int end ); 
#ifdef __cplusplus
}
#endif
#endif
