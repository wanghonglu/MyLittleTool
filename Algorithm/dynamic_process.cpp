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
/*
 *   leetcode  322
 * */
std::vector<int>coins ={20,10,5,1};
int  GetMinCoin( int number )
{
    std::vector<int> result( number+1,0 );
    for( int i=1;i<=number;i++ )
    {
        int min = INT_MAX;
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
        result[i]=min == INT_MAX?-1:min+1;
    }
    return result[number];
}
int dynamic_process_test(int argc,char**argv )
{
    if( argc<2 )
        return -1;
    std::cout<<atoi(argv[1])<<" 元 需要 "<<GetMinCoin(atoi(argv[1]))<<" 个 "<<std::endl;
    return 0;
}
