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
 *  如果选择的基准数是left 然后先从右边开始遍历(因为空位最开始在左边）
 *  找到一个小于基准数的数字 塞到这个空位上 然后最右边的地方又空着了
 *  然后再从左边开始便利，找到大于基准数的数塞到右边空位上
 *  然后左边又有空位了。。。。。
 *  最终遍历结束 start1idx 和start2idx是相等的 这个地方也是空位 把最开始缓存的数填进去即可
 *  如果左边空位填上右边有空位
 *  然后遍历左边都没有满足条件的值 左边会累加直到等于右值 即替换两者皆可
 *  最坏的情况就是原来就是有序的 拆分开来的两个数组分别是1 和n-1个元素 这样时间复杂度会被替换成n^2
 *  所以有三数取中法来优化
 *  切记 
 *      下面的函数里面 left right千万别用size_t 存 因为size_t 无符号 0-1 得到的是一个巨大的整数 会导致循环退不出去 切记切记  size_t 不要滥用!!!!!
 *
 *  这里我采用的都是枢纽变动的  还有枢纽不变的(这种后面可以实现一个)
 *  todo:
 *      快排的尾递归修改  啥叫尾递归？？？
 *      快排划分出三部分 大于 等于 小于 这样对于有大量重复元素的情况会好很多 ，，，，
 *      内容真多啊
 *
 *      尾递归: stackoverflow上这个回答比较好:https://stackoverflow.com/questions/33923/what-is-tail-recursion/37010#37010
 *      尾递归: https://blog.csdn.net/qq_38289815/article/details/105487879
 *
 *      快排为什么比堆排序快: http://mindhacks.cn/2008/06/13/why-is-quicksort-so-quick/
 *      堆排每次拿堆顶跟堆底比较 两者其实差异很大 因为cpu的cashe的原因 数组中相距很远的两个值不容易命中cashe 得从内存中取 比较耗时
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
void
choose_right( int*ptr, int left, int right );
// 快排的去重 针对重复元素过多的情况优化
// 将待排序数组划分为三部分 小于 等于 大于 三部分
void
better_quick_sort2( int*ptr, int len );
void
_better_quick_sort2( int*ptr, int left, int right );
// 查找跟pos 重复的元素
void
deal_repeat( int*ptr, int left, int pos, int right, int *pos_left, int* pos_right );

//快排的尾递归优化
//其实还是不太明白 为什么把第二次递归放在循环里面就能实现尾递归优化.... 试试看能不能快吧

void 
better_quick_sort3( int*ptr, int len );
void
_better_quick_sort3(int*ptr, int left, int right );
int
partition(int*ptr, int left, int right );









#ifdef __cplusplus
}
#endif
#endif

