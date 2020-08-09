/*
 * 有 n 个城市通过 m 个航班连接。每个航班都从城市 u 开始，以价格 w 抵达 v。
 *
 * 现在给定所有的城市和航班，以及出发城市 src 和目的地 dst，你的任务是找到从 src 到 dst 最多经过 k 站中转的最便宜的价格。 如果没有这样的路线，则输出 -1。
 *
 *
 * 输入: 
 * n = 3, edges = [[0,1,100],[1,2,100],[0,2,500]]
 * src = 0, dst = 2, k = 1
 * 输出: 200
 * 解释: 
 * 城市航班图如下
 *          0
 *     (100)     (500)
 *
 *      1  (100)    2
 *
 * n 范围是 [1, 100]，城市标签从 0 到 n - 1.
 * 航班数量范围是 [0, n * (n - 1) / 2].
 * 每个航班的格式 (src, dst, price).
 * 每个航班的价格范围是 [1, 10000].
 * k 范围是 [0, n - 1].
 * 航班没有重复，且不存在环路
 *
 *
 * */
#include<vector>
#include<iostream>
using namespace std;
class Solution {
    public:
        int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
            //dp数组 每个城市到另一个城市的最低花费 dp[i][j] 从i到j的最低花费
            if( flights.empty() )
                return -1;
            if( src>flights.size() ||dst>flights.size() )
                return -1;
            //统计所有相互之间的航班 不可达设置为-1
            std::vector<std::vector<int>> Allfly( n, std::vector<int>(n,-1 ) );
            for( int i=0;i<flights.size();++i )
            {
                if( flights[i].size()==3 )
                {
                    int s = flights[i][0],d=flights[i][1];
                    if( s<n && d<n )
                        Allfly[s][d]=flights[i][2];
                }

            }

            int min = Allfly[src][dst];//有的话 就有 没有就是-1
            for( int i=1;i<=K;++i )
            {
                if( Allfly[src][src+i]!=-1 && Allfly[src+1][dst]!=-1 && min>(Allfly[src][src+i]+Allfly[src+1][dst]))
                    min = Allfly[src][src+i]+Allfly[src+1][dst];
            }

            return min;
        }
};

