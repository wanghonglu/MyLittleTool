#ifndef __ALGORITHM_HPP
#define __ALGORITHM_HPP
#include<string>
#include<functional>
#include<memory>
#include<map>
#include<iostream>
#include<cstdint>
#include<algorithm>
#include<limits>
#include<assert.h>
#include<vector>
#include   "BaseAlgorithm.hpp"
#include "CommonDataStruct.hpp"
#include<cstring>
#include "Nqueue_Quest.hpp"
#include "GetMinCoinQuest.hpp"
#include "find_unique_number.hpp"
#include "ListAgorithm.hpp"
#include "Hanoi.hpp"
#include "maxSubArray.hpp"
#include "TopKFrequent.hpp"
#include "Sigle_Stack.hpp"



class TestSolutionFactory{
public:
    static TestSolutionFactory& Instant()
    {
        static TestSolutionFactory factory;
        return factory;
    }
    void Regiest(int key,AlgorithmSolution* t)
    {
        m_factory[key]=t;
    }
    AlgorithmSolution* GetOne(int key )
    {
        if(m_factory.count(key))
            return m_factory[key];
        throw std::runtime_error("no Test Solution "+std::to_string(key));
    }
    void PrintAll()
    {
        std::cout<<"usage: "<<std::endl;
        for( const auto& one:m_factory )
        {
            std::cout<<one.first<<"   "<<one.second->m_algorithmName<<std::endl;
        }
    }

private:
    TestSolutionFactory()
    {}
    ~TestSolutionFactory()
    {}
    TestSolutionFactory(const TestSolutionFactory&)=delete;
    TestSolutionFactory& operator =(const TestSolutionFactory& )=delete;
    std::map<int,AlgorithmSolution*>    m_factory;
};
//全局范围内不能进行函数调用，所以必须把注册放到构造函数里面去
#define CONCAT_(a,b) a##b
#define CONCAT(a,b) CONCAT_(a,b)
//不直接用classname是因为会有模板的情况出现
#define RegistAlgorithm(classname)   \
class CONCAT(RegistCalss,CONCAT(__FUNCTION__,__LINE__)){                  \
    public:                               \
    CONCAT(RegistCalss,CONCAT(__FUNCTION__,__LINE__))()                         \
    {                                     \
        TestSolutionFactory::Instant().Regiest(m_alg.m_index,&m_alg);\
    }                                     \
    private:                              \
    classname     m_alg;                  \
};\
static CONCAT(RegistCalss,CONCAT(__FUNCTION__,__LINE__)) CONCAT(s_,CONCAT(RegistCalss,CONCAT(__FUNCTION__,__LINE__)));

#define GetOneTest(type)  TestSolutionFactory::Instant().GetOne(type)
#define GetHelp()  TestSolutionFactory::Instant().PrintAll()

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
#endif