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
//8皇后
RegistAlgorithm(NQueueQuest<8>)
//找零钱
RegistAlgorithm(GetMinCoinQuest<41>)
//找唯一不重复的
RegistAlgorithm(FindUnique_number)
//链表倒数第K个值
RegistAlgorithm(ListGetKthFromEnd)
//汉诺塔问题
RegistAlgorithm(HanoiTowerAgorithm<10>)
//查找链表的环
RegistAlgorithm(FindListCircle)
//和最大子数组
RegistAlgorithm(MaxSumArray)
//乘积最大子数组
RegistAlgorithm(MaxProductArray)
//统计出现频率前K高的元素
RegistAlgorithm(TopKFrequent<3>)
//统计图最大矩形问题
RegistAlgorithm(LargestRectanleArea)
//排队向右看问题
RegistAlgorithm(InorderLineUp)
//下一个最高问题问题
RegistAlgorithm(NextHighestTemperture)
//接雨水问题
RegistAlgorithm(GetRainQuest)
//合并链表
RegistAlgorithm(MergeOrderedList)
//反转链表
RegistAlgorithm(ReserverList)
int main(int argc,char**argv )
{
    int opt =-1;
    try
    {
       if( argc>1 )
            opt = atoi(argv[1]);
       AlgorithmSolution* test = GetOneTest(opt);
       test->Solution();
    }
    catch(const std::exception& e)
    {
        GetHelp();
        std::cerr << e.what() << '\n';
    }
    
}