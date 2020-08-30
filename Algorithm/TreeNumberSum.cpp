#include "BaseAlgorithm.hpp"
/*
    3数字之和
    leetcode 15
    给你一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？请你找出所有满足条件且不重复的三元组。

    注意：答案中不可以包含重复的三元组
*/
#include<unordered_map>
class ThreeSum:public AlgorithmSolution
{
    public:
    ThreeSum()
    {
        m_algorithmName = "三数之和";
    }
    void Solution()override
    {
        std::vector<int> v={-1, 0, 1, 2, -1, -4};
        std::vector<std::vector<int>>ret= threeSum(v);
        std::cout<<"原数组: "<<std::endl;
        PrintArray(v);
        std::cout<<"能组成三个数相加等于0的所有组合为:"<<std::endl;
        for(int i=0;i<ret.size();++i )
            PrintArray(ret[i]);
    }
    std::vector<std::vector<int>> threeSum( std::vector<int>& nums )
    {
        //双指针法
        std::vector<std::vector<int>> ret;
        if( nums.size()<3 )
            return ret;
        std::sort(nums.begin(),nums.end());
        for( int i=0;i<nums.size()-2;++i )
        {
            if(nums[i]>0)
                return ret;
            if( i>0 && nums[i]==nums[i-1])//去重
                continue;
            int left = i+1;
            int right = nums.size()-1;
            while(left<right)
            {
                int temp = nums[i]+nums[left]+nums[right];
                if(temp>0)
                    right--;
                else if(temp<0)
                    left++;
                else
                {
                    ret.push_back(std::vector<int>{nums[i],nums[left],nums[right]});
                    left++;
                    right--;
                    //只要一个值变了 i不变 另一个必然得变不然不可能再次等于0 这里需要去重
                    while(left<right && nums[left]==nums[left-1])
                        left++;
                    while(left<right && nums[right] == nums[right+1])
                        right--;
                }
            }
        }
        return ret;
    }
};
RegistAlgorithm(ThreeSum)
/*
    两数之和  leetcode 1
    
    给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那 两个 整数，并返回他们的数组下标。

    你可以假设每种输入只会对应一个答案。但是，数组中同一个元素不能使用两遍。
    //双指针 用的时候必须要保证数组是有序的 如果是返回原数组下标那么问题可能就不太一样了
    这个用哈希表更加简单
*/
template<int N>
class TwoSum:public AlgorithmSolution{
    public:
    TwoSum()
    {
        m_algorithmName="两数之和";
    }
    void Solution()override
    {
        std::vector<int> v={3,2,4};
        std::vector<int> ret = twoSum(v,N);
        std::cout<<"原数组:";
        PrintArray(v);
        std::cout<<"和为："<<N<<" 的两个数的下标为: ";
        PrintArray(ret);
    }
    std::vector<int>twoSum(std::vector<int>& nums,int target )
    {
        // std::vector<int> ret;
        // if(nums.size()<2 )
        //     return ret;
        // std::vector<int> numbers(nums);
        // std::sort(numbers.begin(),numbers.end());
        // int left = 0,right=numbers.size()-1;
        // while(left<right)
        // {
        //     if(numbers[left]>target )
        //         return ret;
        //     int temp = numbers[left]+numbers[right];
        //     if(temp == target )
        //         break;
        //     if(temp<target)
        //         left++;
        //     else 
        //         right--;
        // }
        // int left_idx=-1,right_idx=-1;
        // for( int i=0;i<nums.size();i++)
        // {
        //     if(left_idx == -1 && nums[i] == numbers[left])
        //         left_idx = i;
        //     else if( right_idx == -1 && nums[i] == numbers[right])
        //         right_idx = i;
        //     if( left_idx !=-1 && right_idx !=-1 )
        //         break;
        // }
        // return std::vector<int>{left_idx,right_idx};
        //这个用哈希表更简单!!!
        std::unordered_map<int,int> numbers;
        for( int i=0;i<nums.size();i++ )
        {
            int temp = target-nums[i];
            if(numbers.count(temp) )
                return std::vector<int>{numbers[temp], i };
            numbers[nums[i]] = i;
        }
        return std::vector<int>{};
    }
};
RegistAlgorithm(TwoSum<6>)