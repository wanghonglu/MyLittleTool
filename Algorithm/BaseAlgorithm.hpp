#ifndef __BASEALGORITHM__HPP__
#define __BASEALGORITHM__HPP__
#include<cstdio>
#include<string>
#include<iostream>
#include "CommonDataStruct.hpp"
class AlgorithmSolution{
    public:
        static  size_t s_index;
        AlgorithmSolution(){
            m_index = AlgorithmSolution::s_index++;
        }
        virtual void  Solution()
        {
            std::cout<<m_algorithmName<<std::endl;
        }
        std::string   m_algorithmName="未知算法";
        size_t        m_index=0;
        virtual ~AlgorithmSolution(){}

};

size_t AlgorithmSolution::s_index=0;
#define PrintArray(a) do{\
    std::for_each(a.begin(),a.end(),[](decltype(a[0]) t ){std::cout<<t<<" ";});\
    std::cout<<std::endl;\
}while(0)
#endif