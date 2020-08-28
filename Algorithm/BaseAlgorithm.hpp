#ifndef __BASEALGORITHM__HPP__
#define __BASEALGORITHM__HPP__
#include<cstdio>
#include<string>
#include<iostream>
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

//这种方式没办法 处理模板类
// #define RegistAlgorithm(classname)   \
// class CONCAT(RegistCalss,CONCAT(classname,__LINE__)){                  \
//     public:                               \
//     CONCAT(RegistCalss,CONCAT(classname,__LINE__))()                         \
//     {                                     \
//         TestSolutionFactory::Instant().Regiest(m_alg.m_index,&m_alg);\
//     }                                     \
//     private:                              \
//     classname     m_alg;                  \
// };\
// static CONCAT(RegistCalss,CONCAT(classname,__LINE__)) CONCAT(s_,CONCAT(RegistCalss,CONCAT(classname,__LINE__)));
//这种方式可以处理模板类，如果定义在同一个文件中 则通过LINE可以区分，static是单个模块可见的 名字就算重复也无所谓
//构造函数中进行注册 
//这么写也会有名字重复的可能，比如一个main.cpp包含所有文件 俩文件中可能有处于同一行的，
//#define RegistAlgorithm(classname)  static register_t<classname> CONCAT(s_static_regist, __LINE__);
//终极解决方案
    //定义一个注册类
template<typename T>
struct algorithm_register
{
    explicit algorithm_register()
    {
        T * temp = new T();
        TestSolutionFactory::Instant().Regiest( temp->m_index,temp );
    }
};
template<typename T>
struct algorithm_register_agent{
    static algorithm_register<T> help;
};
//终极解决方案 就是这个 注意 这里需要模板特例化 也就是那个template<>不能少 少了就编译报错了
//而且这个必须得初始化 不初始化则不会调用构造函数 坑爹啊啊啊啊啊啊啊啊啊啊 
#define RegistAlgorithm(classname) \
 template<> algorithm_register<classname> algorithm_register_agent<classname>::help =algorithm_register<classname>() ;


#define GetOneTest(type)  TestSolutionFactory::Instant().GetOne(type)
#define GetHelp()  TestSolutionFactory::Instant().PrintAll()
#endif