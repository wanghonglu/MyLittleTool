#include "Algorithm_test.hpp"
#include<cstring>
#include "Nqueue_Quest.hpp"
#include "GetMinCoinQuest.hpp"
#include "find_unique_number.hpp"
#include "ListAgorithm.hpp"
#include "Hanoi.hpp"
#include "maxSubArray.hpp"
#include "TopKFrequent.hpp"
#include "Sigle_Stack.hpp"
enum{
    NQueue_Quest,//8皇后问题 回溯算法
    GetMinCoin_Quest,//找零钱问题，动态规划
    Find_Unique,//找出唯一不重复的数
    FindKthFromEnd,//找到链表的倒数第K个值
    Hanoi,//汉诺塔问题
    Find_List_Circle,//查找链表环
    MaxSum_SubArray,//和最大子数组
    MaxProduct_SubArray,//乘积最大子数组
    TopK_Frent,//统计出现频率前K高的元素
    LargetRectangle_Area,//统计图最大矩形面积
    InOrder_LineUp,//排队向右看问题
    NextHighest_Temperature,//下一个最高温度问题
    GetRain_Quest,//接雨水问题
    MergeList_Quest,//合并链表
};
//8皇后
RegistAlgorithm(NQueueQuest<8>,NQueue_Quest)
//找零钱
RegistAlgorithm(GetMinCoinQuest<41>,GetMinCoin_Quest)
//找唯一不重复的
RegistAlgorithm(FindUnique_number,Find_Unique)
//链表倒数第K个值
RegistAlgorithm(ListGetKthFromEnd,FindKthFromEnd)
//汉诺塔问题
RegistAlgorithm(HanoiTowerAgorithm<10>,Hanoi)
//查找链表的环
RegistAlgorithm(FindListCircle,Find_List_Circle)
//和最大子数组
RegistAlgorithm(MaxSumArray,MaxSum_SubArray)
//乘积最大子数组
RegistAlgorithm(MaxProductArray,MaxProduct_SubArray)
//统计出现频率前K高的元素
RegistAlgorithm(TopKFrequent<3>,TopK_Frent)
//统计图最大矩形问题
RegistAlgorithm(LargestRectanleArea,LargetRectangle_Area)
//排队向右看问题
RegistAlgorithm(InorderLineUp,InOrder_LineUp)
//下一个最高问题问题
RegistAlgorithm(NextHighestTemperture,NextHighest_Temperature)
//接雨水问题
RegistAlgorithm(GetRainQuest,GetRain_Quest)
//合并链表
RegistAlgorithm(MergeOrderedList,MergeList_Quest)
int main(int argc,char**argv )
{
    int opt =-1;
    try
    {
       if( argc>1 )
            opt = atoi(argv[1]);
       AgorithmSolution* test = GetOneTest(opt);
       test->Solution();
    }
    catch(const std::exception& e)
    {
        GetHelp();
        std::cerr << e.what() << '\n';
    }
    
}