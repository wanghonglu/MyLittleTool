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
#include<vector>
#include<iostream>
#include<cstring>
#include<set>
#include<algorithm>
using namespace std;
bool isRight( std::vector<std::vector<int>>& nums, int row, int col )
{
    //判断当前列有没有元素
    size_t n = nums.size();
    for( int i=0;i<n;++i )
    {
        if( nums[i][col] == 1 )//表示有皇后
            return false;
    }
    //判断row col所在的元素的左上角有没有皇后 row-1,col-1 依次往上递增
    for( int i=row-1,j=col-1;i>=0&&j>=0;--i,--j )
    {
        if( nums[i][j]==1 )
            return false;
    }
    //判断row col所在的元素右上角有没有皇后 row-1,col+1 依次网上递增
    for( int i=row-1,j=col+1;i>=0&&j<n;i--,j++ )
    {
        if( nums[i][j]==1 )
            return false;
    }
    //只所以不判断下面是因为我们从第一层开始的,下面是没有元素的
    return true;
}
void printQueen( const std::vector<std::vector<int>>&nums )
{
    for(int i=0;i<nums.size();++i )
    {
        cout<<"\t";
        for( int j=0;j<nums[i].size();++j )
        {
            if( nums[i][j] )
                cout<<" Q ";
            else
                cout<<" * ";

        }
        cout<<endl;
    }
    cout<<endl;
}
void NQueen( std::vector<std::vector<int>>& nums, int row, int& result  )
{
    if( row == nums.size() )//表示最后一行也满足
    {
        result++;
        //printQueen(nums);
        return;
    }
    for( int i=0;i<nums[row].size();++i )
    {
        if( !isRight(nums, row, i ) )
            continue;
        //选择
        nums[row][i]=1;
        //当前row 当前列i 目前位置是满足皇后的要求的,找下一行
        NQueen(nums,row+1,result );//下一行找 直到找到/或者没找到 回溯回来,重置刚才的选择,继续下一个选择
        //重置选择 回溯
        nums[row][i]=0;
    }
}
void Fullarange(const std::vector<int>&numbers, std::vector<int>&select,  std::vector<std::vector<int>>&result )
{
    if( select.size() == numbers.size() )//满足条件 即已经选择的元素 跟给出的元素相等 
    {
        result.push_back(select);
        return;
    }
    //从第一个数字开始找所有的元素
    for( int i=0;i<numbers.size();i++ )
    {
        if( std::find(select.begin(),select.end(),numbers[i])!=select.end() )
            continue;
        select.push_back( numbers[i] );
        Fullarange(numbers,select,result );
        select.pop_back();
    }
}
int main(int argc,char**argv )
{
    if( argc<2 )
        return -1;
    size_t Count=atoi(argv[1]);
    int result=0;
    std::vector<std::vector<int>> numbers( Count, std::vector<int>(Count,0 )) ;
    NQueen( numbers, 0, result ); 
    cout<<Count<<" 皇后,总共  "<<result<<" 种摆放法"<<endl;

    std::vector<int> nums{1,2,3,4};
    std::vector<int> v;
    std::vector<std::vector<int>>ret;
    Fullarange(nums,v,ret );
    std::for_each( ret.begin(),ret.end(),[](const std::vector<int>& a ){ std::for_each(a.begin(),a.end(),[](int a){cout<<a;});cout<<endl;});
}
