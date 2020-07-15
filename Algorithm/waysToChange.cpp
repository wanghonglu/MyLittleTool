/*
 *
 * 硬币。给定数量不限的硬币，币值为25分、10分、5分和1分，编写代码计算n分有几种表示法。(结果可能会很大，你需要将结果模上1000000007)
 *  输入: n = 10
 *   输出：4
 *    解释: 有四种方式可以凑成总金额:
 *    10=10
 *    10=5+5
 *    10=5+1+1+1+1+1
 *    10=1+1+1+1+1+1+1+1+1+1
 *
 *    动态规划
 *      问题可以拆解成( f(n)=f(n-1)+f(n-5)+f(n-10)+f(n-25)
 * */
#include<vector>
#include<iostream>
using namespace std;
 int waysToChange(int n) {

    if( n<=0 )
        return 0;
    std::vector<int> Change(n+4);
    Change[0]=1;
    for( int i=1;i<=n;i++ )
    {
        if( i < 5)
            Change[i]=Change[i-1];
        else if( i<10 &&i>=5 )
            Change[i]=Change[i-5]+Change[i-1];
        else if(i>=10 && i<25 )
            Change[i]=Change[i-1]+Change[i-5]+Change[i-10]; 


    }
 }
