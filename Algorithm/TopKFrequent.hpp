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
 *  思路:
 *      先统计每个元素出现的次数,耗时O(n)
 *      然后建立一个小顶堆( 好像但凡 一堆数据里面找出前k大类似的描述 都可以用优先级队列来做)
 *      k个元素每次移动耗时 logk
 *      总体时间复杂度是小于 nlogn的
 * */
#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<unordered_map>
#include<map>
#include "Algorithm_test.hpp"
using namespace std;
template<size_t N>
class TopKFrequent:public AgorithmSolution{
public:
    TopKFrequent()
    {
        m_algorithmName = "统计出现频率前K高的元素";
        m_nk = N;
    }
    void Solution()override
    {
        std::vector<int> a={1,1,1,8,6,6,6,6,6,8,8,8,8,8,2,2,3};
        auto result = topKFrequent(a,m_nk);
        std::cout<<"原数组 ";
        PrintArray(a);
        std::cout<<"中出现频率前 "<<m_nk<<" 高的元素为: ";
        PrintArray(result);
    }

    std::vector<int> topKFrequent(vector<int>& nums, int k) {
        std::vector<int> result;
        if( nums.empty())
            return result;

        std::unordered_map<int,int> count;
        //最小堆 堆顶最小
        auto cmp=[](const std::pair<int,int>& t1, const std::pair<int,int>& t2 ){
                return t1.second>t2.second;
            };
        PriorityQueue<std::pair<int,int>, decltype(cmp)>count_sort(cmp);
        for( int i=0;i<nums.size();++i )
        {
            count[nums[i]]++;
        }

        for( auto& one:count )
        {
            if( count_sort.size()<k )
            {
                count_sort.push(one );
            }
            else
            {
                if( count_sort.top().second<one.second )
                {
                    count_sort.pop();
                    count_sort.push( one );
                }
            }

        }
        while( !count_sort.empty() )
        {
            result.push_back( count_sort.top().first );
            count_sort.pop();
        }

        return result; 
    }
    size_t  m_nk;
};

