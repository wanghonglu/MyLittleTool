
#include "BaseAlgorithm.hpp"
/*
    盛最多水的容器问题
    leetcode11
    给你 n 个非负整数 a1，a2，...，an，每个数代表坐标中的一个点 (i, ai) 。
    在坐标内画 n 条垂直线，垂直线 i 的两个端点分别为 (i, ai) 和 (i, 0)。找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
    [1,8,6,2,5,4,8,3,7]

    最笨的方法是针对 每个i 去从左边和右边开始找大于等于i的值 相应的算出left right边界 然后计算面积
    这样的时间复杂度  几乎是O(n2)
    但是 可以左右两边开始移动，以两者更小的值为高计算出相应的面积

    相当于最终所求面积的 基地从最宽开始收缩，收缩的规则是把更小的边做相应的收缩
*/
class MaxAreaWalter:public AlgorithmSolution
{
    public:
    MaxAreaWalter()
    {
        m_algorithmName = "盛最多数容器问题";
    }
    void Solution()override
    {
        std::vector<int> v={1,8,6,2,5,4,8,3,7};
        //int result  = maxArea1(v);
        int result = maxArea(v);
        std::cout<<"原数组: ";
        PrintArray(v);
        std::cout<<"最大盛水面积: "<<result<<std::endl;
    }
    //方法1 时间复杂度O(n2)
    int maxArea1(std::vector<int>& height )
    {
        if( height.size()<=1 )
            return 0;
        int result =0;
        for( int i=0;i<height.size();++i )
        {
            int left=0,right =height.size()-1,area=0;
            for( int j=0;j<=i;j++ )
            {
                if( height[j]>=height[i])
                {
                    left = j;
                    break;
                }
            }
            for( int k=height.size()-1;k>=i;k-- )
            {
                if(height[k]>=height[i])
                {
                    right = k;
                    break;
                }
            }
            area = (right-left)*height[i];
            if( area>result )
                result = area;
        }
        return result;
    }
    //方法2  时间复杂度O(n)
    int maxArea(std::vector<int>& height )
    {
        if( height.size()<=1 )
            return 0;
        int left =0,right = height.size()-1,result =0;;
        while( left<right )
        {
            int l=0,h=0;
            l = right-left;
            //宽度一定的情况下 面积以最短的那块为准
            if(height[left]<=height[right])
            {
                h = height[left];
                left++;
            }
            else
            {
                h = height[right];
                right--;
            }
            if( l*h>result )
                result = l*h;
        }
        return result;
    }
};
RegistAlgorithm(MaxAreaWalter)