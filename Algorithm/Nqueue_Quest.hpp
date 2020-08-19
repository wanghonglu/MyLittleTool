/*
 *  回溯算法
 *
 *  回溯算法是一种递归表现形式
 *  大概就是每一种选择去试,结果满足记录结果,不满足取消选择,继续选择别的内容
 *  void func( 路径,选择列表 )
 *  if 满足条件
 *      记录结果
 *      return
 *  for 选择 in 选择列表 
 *      做选择
 *      func(路径,选择列表 )
 *      撤销选择
 *
 *  这里通过两个问题来强化这一个算法
 *  N皇后问题:
 *  在n×n格的国际象棋上摆放n个皇后，使其不能互相攻击，即任意两个皇后都不能处于同一行、同一列或同一斜线上，问有多少种摆法
 *  全排列问题
 *  给出特定的数字 字母 求出所有的排列
 * */
/*
 *  N皇后问题 从第一行开始 选择一个元素,即将该置为1 
 *  判断 当前列有没有元素(因为每行只取一个,所以保证了每行只有一个
 *       因为是从第一行开始的,所以下面是暂时没有皇后的,需要的判断就是 左上和左下
 *       满足条件选择那一位 置1,然后判断下一行,直到每一行都满足,回溯回去,撤销选择(置0),然后选择下一位
 *       这个的计算量其实很大 n^n次方 
 *       
 * */
#ifndef _NQUEEN_QUEST__H
#define _NQUEEN_QUEST__H
#include<vector>
#include<iostream>
#include<cstring>
#include<set>
#include<algorithm>
#include "BaseAlgorithm.hpp"
using namespace std;
template<size_t N>
class NQueueQuest:public AlgorithmSolution{
    public:
    NQueueQuest():m_base(N,std::vector<int>(N,0))
    {
        m_algorithmName = "N皇后问题，回溯算法";
        m_nQueue = N;
    }
    virtual void Solution()override{
        AlgorithmSolution::Solution();
        NQueen(0);
        std::cout<<"总共 "<<m_nQueue<<" 个皇后 共 "<<m_result<<" 个摆法"<<std::endl;
    }
    void NQueen(int row )
    {
        if( row == m_nQueue )
        {
            ++m_result;
            printQueen();
            return;
        }
        for( int i=0;i<m_nQueue;++i )
        {
            if( !isRight(row,i))
                continue;
            //选择
            m_base[row][i]=1;
            //当前row 当前列i 目前位置是满足皇后的要求的,找下一行
            NQueen(row+1 );//下一行找 直到找到/或者没找到 回溯回来,重置刚才的选择,继续下一个选择
            //重置选择 回溯
            m_base[row][i]=0;
        }
    }
    bool isRight(int row, int col )
    {
        //判断当前列有没有元素
        size_t n = m_base.size();
        for( int i=0;i<n;++i )
        {
            if( m_base[i][col] == 1 )//表示有皇后
                return false;
        }
        //判断row col所在的元素的左上角有没有皇后 row-1,col-1 依次往上递增
        for( int i=row-1,j=col-1;i>=0&&j>=0;--i,--j )
        {
            if( m_base[i][j]==1 )
                return false;
        }
        //判断row col所在的元素右上角有没有皇后 row-1,col+1 依次网上递增
        for( int i=row-1,j=col+1;i>=0&&j<n;i--,j++ )
        {
            if( m_base[i][j]==1 )
                return false;
        }
        //只所以不判断下面是因为我们从第一层开始的,下面是没有元素的
        return true;
    }
    void printQueen( )
    {   
        if(m_nQueue>4 )
            return;
        for(int i=0;i<m_base.size();++i )
        {
            cout<<"\t";
            for( int j=0;j<m_base[i].size();++j )
            {
                if( m_base[i][j] )
                    cout<<" Q ";
                else
                    cout<<" * ";

            }
            cout<<endl;
        }
        cout<<endl;
    }   
    private:
    size_t     m_nQueue;
    int        m_result;
    std::vector<std::vector<int>> m_base;
};

#endif