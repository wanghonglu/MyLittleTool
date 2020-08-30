#include"BaseAlgorithm.hpp"
/*
    leetcode 14
    剪绳子
    给你一根长度为 n 的绳子，请把绳子剪成整数长度的 m 段（m、n都是整数，n>1并且m>1），
    每段绳子的长度记为 k[0],k[1]...k[m - 1] 。请问 k[0]*k[1]*...*k[m - 1] 可能的最大乘积是多少？
    例如，当绳子的长度是8时，我们把它剪成长度分别为2、3、3的三段，此时得到的最大乘积是18。
    输入: 10
    输出: 36
    解释: 10 = 3 + 3 + 4, 3 × 3 × 4 = 36
    
    这个问题可以用动态规划的思路去解决
    这个题目 有点坑啊 一堆数学证明 尽量划分成3的时候乘积是最大的
    求极限 最后能求出个 x=(n/e) 每段绳子为n/e时 乘积最大  即n取3

    但是如果最终剩下4的时候其实应该22分

    答案需要取模 1e9+7（1000000007），如计算初始结果为：1000000008，请返回 1。
*/
template<size_t N>
class CuttingRope:public AlgorithmSolution{
    public:
    CuttingRope()
    {
        m_algorithmName = "剪绳子 凑最大乘积问题";
    }
    void Solution()override
    {
        int ret = cuttingRope(N);
        std::cout<<"绳子总共长: "<<N<<" 能拼凑出的最大乘积为: "<<ret<<std::endl;
    }
    int cuttingRope(int n) 
    {
        if(n <=3 )
            return n-1;
        //4以上 优先每段分3米 剩下的4段 按照22分 3段乘以3 2段乘以2
        long long res = 1;
        while(n>4)
        {
            n-=3;
            res = res*3;//快速幂算法  即 (a^b)%m = a%m*b%m)%m
            res%=1000000007;//这地方必须要先对结果取余 不然后面就溢出了
            //快速幂算法 参考https://blog.csdn.net/qq_19782019/article/details/85621386
            //指数爆炸问题
        }
        res = n*res;
        return res%1000000007;

    }
};
RegistAlgorithm(CuttingRope<120>)