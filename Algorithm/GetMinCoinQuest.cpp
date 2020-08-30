/*
 *  动态规划 将问题可以拆分成小问题,无线细分最终找到答案 而且 i+1的结果 不会影响i的结果
 *  叫 无后效性
 *  比如斐波那契数列 就可以用动态规划的思想解决
 *  再比如青蛙跳台阶,一次能跳一个台阶或者2个 求100个台阶总共有多少中算法 这样的话 
 *  首先 f(1)=1;(第一个台阶只有这一种跳法
 *  f(2)=2;(第二个台阶 两种 直接跳或者一个一个跳
 *   
 *  那么f(3) 可以想成 从第二个台阶跳一次到第三个台阶 也可以从第一个台阶跳两个台阶
 *  即f(3)=f(2)+f(1);
 *
 *  f(100) = f(99)+f(98);
 *
 *  有个硬币问题, 加入给41个硬币 总共的有以下几种面值 1 5 10 25 50 
 *  这个可以用贪心算法,贪心算法只能是局部的最优解 不能是全局的最优解
 *  这里也可以先写出贪心算法,正确的做法应该是动态规划
 *  比如如果面值有20的 则按照贪心算法得到的结果 25 10 5 1 实际上 20 20 1也可以
 *
 *  可以通过取 差一元 5元 10元 25元 这四个中的最小值
 *
 * */
//硬币数组 从最大开始
#include<vector>
#include<iostream>
#include<set>
#include<cstring>
#include "BaseAlgorithm.hpp"
/*
 *   leetcode  322
 * */
template<size_t N>
class GetMinCoinQuest:public AlgorithmSolution{
    public:
    GetMinCoinQuest()
    {
        m_algorithmName = "找零钱问题 动态规划";
        m_counts = N;
    }
    void Solution()override
    {
        AlgorithmSolution::Solution();

        std::vector<int>coins ={20,10,5,1};
        std::vector<int> result( m_counts+1,0 );

        for( int i=1;i<=m_counts;i++ )
        {
            int min = std::numeric_limits<int>::max();
            for( int j=0;j<coins.size();j++ )
            {
                if( i == coins[j] )
                {
                    min = 0;
                    break;
                }
                if( i>coins[j] &&result[i-coins[j]]!=-1 &&  min>result[i-coins[j]] )
                    min = result[i-coins[j]];
            }
            result[i]=min == std::numeric_limits<int>::max()?-1:min+1;
        }
        std::cout<<" 给出可供选择的硬币 "<<std::endl;
        for( int a:coins )
        {
            std::cout<<a<<" ";
        }
        std::cout<<std::endl;
        std::cout<<" 需要凑出的零钱 "<<m_counts<<std::endl;
        std::cout<<" 需要的硬币数量 "<<result[m_counts]<<std::endl;
    }
    private:
    size_t             m_counts;//总共需要的零钱
};
RegistAlgorithm(GetMinCoinQuest<41>)
/*
    给定不同面额的硬币和一个总金额。写出函数来计算可以凑成总金额的硬币组合数。假设每一种面额的硬币有无限个。 
    amount = 5, coins = [1, 2, 5]
    输出: 4
    解释: 有四种方式可以凑成总金额:
    5=5
    5=2+2+1
    5=2+1+1+1
    5=1+1+1+1+1   
*/
class GetAllCoin:public AlgorithmSolution{
    public:
    GetAllCoin()
    {
        m_algorithmName="获取所有硬币组合 动态规划";
    }
    void Solution()override
    {
        std::vector<int>coins={2,5};
        int amount=5;
        int result = change(amount,coins);
        std::cout<<"给出的硬币 "<<std::endl;
        PrintArray(coins);
        std::cout<<"总共有 "<<result<<" 种组合,可以凑成 "<<amount<<" 元"<<std::endl;
    }
    int change(int amount, std::vector<int>& coins) {
        if( amount<=0 || coins.empty() )
            return 0;
        std::sort(coins.begin(),coins.end(),std::less<int>());
        std::vector<int> result(amount+1,0);
        for( int i=1;i<=amount;i++ )
        {
            bool  find_equal=false;
            for( auto e:coins )
            {
                if( e == i )
                    find_equal=true;
                //这里其实有重复的给定硬币1 2 3; 比如3元 1元1种 2元2种 3元却不是3种 而是2种即1+1+1 和2+1
                //这里需要去重,和找最大最小还不太一样
                //if(i-e-e>0)说明此时的i-e已经包含了这种模式
                if(i-e>0&&i-2*e<=0)
                    result[i] += result[i-e]+1;
            }
        }
        PrintArray(result);
        return result[amount];
    }
};
RegistAlgorithm(GetAllCoin)