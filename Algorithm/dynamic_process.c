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
 * */
//硬币数组 从最大开始
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int Coin[]={50,25,20,10,5,1};
const int Coinlen = sizeof(Coin)/sizeof(int);
//贪心算法实现 返回每种硬币的数量
typedef struct TypeCoinCost{
    int*  eachCoinCost;
    size_t total_cost;
}TypeCoinCost;

TypeCoinCost*  GreedyCoin( int num )
{
    int result=0;
    TypeCoinCost  *coinCount = (TypeCoinCost*)malloc(sizeof(TypeCoinCost));
    coinCount->eachCoinCost = (int*)malloc(sizeof(int)*Coinlen);
    memset( coinCount->eachCoinCost, 0, sizeof(int)*Coinlen );
    for( int i=0;i<Coinlen;i++ )
    {
        while( num>=Coin[i] )
        {
            coinCount->eachCoinCost[i]++;
            coinCount->total_cost++;
            num -=Coin[i];
        }
    }
    return coinCount;
}
TypeCoinCost* DynamicProcess( int num )
{
    TypeCoinCost *coinCount = (TypeCoinCost*)malloc( sizeof(TypeCoinCost)*num );
    for( int i=1;i<=num;i++ )
    {
       coinCount->eachCoinCost = (int*)malloc(sizeof(int)*Coinlen);
       memset( coinCount->eachCoinCost, 0, sizeof(int)*Coinlen );
       int j=0;
       int find=0;
       for( ;j<Coinlen;j++ )
       {
            if( i == Coin[j] )
            {
                coinCount[i].eachCoinCost[j] = 1;
                coinCount[i].total_cost=1;
                find=1;
                break;
            }

            if( i>Coin[j] )
                break;
       }
       if( find )
           continue;

       //这里是排序 比如 17 这里就是 10的时候跳出来 那么计算就是 min( coinCount[7],coinCount[12],coinCount[16] )+1; 
       int min=1;;
       for( int  k=j;k<Coinlen;k++ )
       {
           if( coinCount[i-Coin[k]].total_cost <= min )
               min = coinCount[i-Coin[k]].total_cost;
       }
       coinCount[i].total_cost = min+1;
    }

    printf( "%d\n", coinCount[num].total_cost );
}
int main(int argc,char**argv )
{
    if( argc<2 )
        return -1;
    int count = atoi(argv[1]);

    TypeCoinCost *ptr = GreedyCoin(count);

    TypeCoinCost *tt = DynamicProcess(count);


    fprintf( stderr, "可供选择的面额" );
    for( int i=0;i<Coinlen;i++ )
    {
        fprintf( stderr, " %d ", Coin[i] );
    }

    fprintf( stderr, "\n" );

    fprintf( stderr, "%d 总共需要硬币: %d ", count, ptr->total_cost );
    for( int i=0;i<Coinlen;i++ )
    {
        if( ptr->eachCoinCost[i]!=0 )
        {
            fprintf( stderr, "%d的硬币%d个 ", Coin[i],ptr->eachCoinCost[i] );
        }
    }
    free(ptr->eachCoinCost);
    free(ptr);
    fprintf( stderr, "\n" );
    return 0;
}
