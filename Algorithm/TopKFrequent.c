/*
 *  给定一个非空的整数数组，返回其中出现频率前 k 高的元素。
 *
 *  输入: nums = [1,1,1,2,2,3], k = 2
 *  输出: [1,2]
 *  提示：
 *
 *  你可以假设给定的 k 总是合理的，且 1 ≤ k ≤ 数组中不相同的元素的个数。
 *  你的算法的时间复杂度必须优于 O(n log n) , n 是数组的大小。
 *  题目数据保证答案唯一，换句话说，数组中前 k 个高频元素的集合是唯一的。
 *  你可以按任意顺序返回答案
 *
 * */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
/*
 *  计数排序 计数数组统计每个元素出现的次数
 * */
int* topKFrequent(int* nums, int numsSize, int k, int* returnSize){
    if( nums == NULL || numsSize<1 || k<1 )
        return NULL;

    int* count = (int*)malloc( sizeof(int)*numsSize );
    memset( count, 0x00, sizeof(int)*numsSize );

    for( int i=0;i<numsSize;i++ )
        count[nums[i]]++;




}
