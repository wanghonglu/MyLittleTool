/*
 *  快速排序
 *
 *  原理：选择一个基准数据，把小于基准数据的数字放在左边 大于基准数据的数放在右边
 *  根据基准数据的选取策略不同，可以将快排分位三类
 *  第一种固定选取 就是把第一个数作为基准数据  或者最后一个数
 *  第二种选取一个随机数，跟上面的差的不太多
 *  第三种 三数取中法，就是 left  mid right 选择一个中间的数
 *
 *  卡了好几天 网上终于找到一个易懂的说法了
 *
 *  选取某个基准数，先缓存下这个数，然后这个位置就相当于空着了
 *  然后先从右边开始遍历(因为空位最开始在左边）
 *  找到一个小于基准数的数字 塞到这个空位上 然后最右边的地方又空着了
 *  然后再从左边开始便利，找到大于基准数的数塞到右边空位上
 *  然后左边又有空位了。。。。。
 *  即循环开始的时候都是左边有空位，所以最后把原来的基准数反填进去的时候
 *  要填在左边那个空位上
 *  如果左边空位填上右边有空位
 *  然后遍历左边都没有满足条件的值 左边会累加直到等于右值 即替换两者皆可
 *  最坏的情况就是原来就是有序的 拆分开来的两个数组分别是1 和n-1个元素 这样时间复杂度会被替换成n^2
 *  所以有三数取中法来优化
 *
 *  todo:
 *      快排的尾递归修改  啥叫尾递归？？？
 *      快排划分出三部分 大于 等于 小于 这样对于有大量重复元素的情况会好很多 ，，，，
 *      内容真多啊
 *
 * */
#ifndef __QUICK_SORT__H
#define __QUICK_SORT__H
#ifdef __cplusplus
extern "C"{
#endif
#include<assert.h>
#include<stdio.h>
//快排 基准值左边
void 
quick_sort(int*ptr, int len );
void 
_quick_sort( int*ptr, int left, int right ); 
//快排 基准值选右边
void
quick_sort2(int*ptr, int len );
void
_quick_sort2(int*ptr, int left, int right );

//三数取中发法
void
better_quick_sort(int*ptr, int len );
void
_better_quick_sort( int*ptr, int left, int right );









#ifdef __cplusplus
}
#endif
#endif

