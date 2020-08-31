#include "BaseAlgorithm.hpp"
/*
幂集。编写一种方法，返回某集合的所有子集。集合中不包含重复的元素。

说明：解集不能包含重复的子集。
示例:

 输入： nums = [1,2,3]
 输出：
[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
    可以用动态规划的思路去解决 针对第n个元素 新的幂集 其实就是在原来的基础上 每个子集中都加上 num[n]
*/
class SubSets:public AlgorithmSolution{
    public:
    SubSets()
    {
        m_algorithmName="求所有子集合";
    }
    void Solution()override
    {
        std::vector<int> v={1,2,3};
        std::vector<std::vector<int>> ret=subsets(v);
        std::cout<<"原数组:";
        PrintArray(v);
        std::cout<<"所有子集合为: ";
        for( auto e:ret )
            PrintArray(e);
        
    }
    std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        std::vector<std::vector<int>> ret;
        ret.push_back(std::vector<int>{});
        for( int i=0;i<nums.size();i++ )
        {
            int have_size = ret.size();
            for( int j=0;j<have_size;j++ )
            {
                std::vector<int> temp = ret[j];
                temp.push_back(nums[i]);
                ret.push_back(temp);
            }
        }
        return ret;
    }
};
RegistAlgorithm(SubSets)