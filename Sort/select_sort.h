#ifndef _SELECT_SORT_H
#define _SELECT_SORT_H
/*
 *  选择排序,每趟选出剩下元素中最小的 或者最大的 
 *  时间复杂度依然是O(n^2) 但比冒泡好在没有频繁的交换操作
 * */
#ifdef __cplusplus
extern "C"{
#endif
void select_sort( int *ptr, int len );

#ifdef __cplusplus
}
#endif
#endif
