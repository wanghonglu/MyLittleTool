/*
 * 插入排序:
 *         针对第i个元素 假设前面 i-1个元素都是有序的,i跟i-1比较 不满足条件往前找满足的位置,直到能
 *         正确放下这个元素
 * 标准库的 std::sort里面的实现 小于32个会有插入排序
 *
 * 希尔排序:
 *         希尔排序是对插入排序的一个优化:
 *         最终还是会进行插入排序的,这点要注意,只不过是为了尽量让数据有序,有序数据的话 希尔排序的复杂度是O(n)
 *
 *         就是按距离拆分组,第一次跨度为 比如16个元素 那么依次拆分为 8 4 2 1 
 *         也就是让 a[0] 和a[8]比较 a[1]和a[9]....a[7]和a[15] 这样比较 数据大概有序
 *         但其实这么做是有盲区的,针对这个增量有各种优化
 *         比如这个分组 按照 2^k-1拆分 质数
 *
 *         希尔排序 能比直接插入排序更好一点
 * */
#ifndef __INSERT_SORT_H
#define __INSERT_SORT_H
#ifdef __cplusplus
extern "C" {
#endif
#include<stdio.h>
#include<assert.h>
#include<math.h>
void
insert_sort( int*ptr, int len );

void
shell_insert_sort( int*ptr, int len );

void
shell_insert_sort_partitionbyprime( int*ptr, int len );



#ifdef __cplusplus
}
#endif
#endif
