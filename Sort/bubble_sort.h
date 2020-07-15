#ifndef __BUBBLE_SORT_H
#define __BUBBLE_SORT_H
#ifdef __cplusplus
extern "C"{
#endif
/*
 *  冒泡排序:
 *  时间复杂度O(n^2)
 *  两两元素比较直到找到最大的 放在最后 第二次两两比较 除了最后一个 直到找到第二大的 
 *  交换频繁,时间复杂度高 空间复杂度O(1)
 * */
void
bubble_sort( int*ptr, int len );
#ifdef __cplusplus
}
#endif
#endif
