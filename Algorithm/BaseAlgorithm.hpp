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


#define PrintArray(a) do{\
    std::for_each(a.begin(),a.end(),[](decltype(a[0]) t ){std::cout<<t<<" ";});\
    std::cout<<std::endl;\
}while(0)
class TestSolutionFactory{
public:
    struct  Algorithm_order;
    static TestSolutionFactory& Instant()
    {
        static TestSolutionFactory factory;
        return factory;
    }
    void Regiest(const Algorithm_order& key ,AlgorithmSolution* t)
    {
        m_factory[key]=t;
    }
    AlgorithmSolution* GetOne(int key )
    {
        for( auto&& e:m_factory )
        {
            if(e.first.m_idx == key )
                return e.second;
        }
        throw std::runtime_error("no Test Solution "+std::to_string(key));
    }
    void PrintAll()
    {
        std::cout<<"usage: "<<std::endl;
        for( const auto& one:m_factory )
        {
            std::cout<<one.first.m_idx<<"\t\t"<<one.second->m_algorithmName<<std::endl;
        }
    }
    //如果这里单纯的用 id来存放的话，每次最后修改号的 不太好找
    // 算法越来越多的时候比较麻烦，这里因为算法在每个文件里，我们用文件的最后修改时间，做为key，去排序
    // 时间越久的排的越靠后
    struct Algorithm_order{
        int m_idx=0;
        int m_file_last_modify_time=0;
        Algorithm_order(int idx,int time_stamp ):m_idx(idx),m_file_last_modify_time(time_stamp){}
        bool operator<(const Algorithm_order& oth )const
        {
            if(m_file_last_modify_time!=oth.m_file_last_modify_time )
                return m_file_last_modify_time<oth.m_file_last_modify_time ;
            return m_idx<oth.m_idx;
        }
    };

private:
    TestSolutionFactory()
    {}
    ~TestSolutionFactory()
    {}
    TestSolutionFactory(const TestSolutionFactory&)=delete;
    TestSolutionFactory& operator =(const TestSolutionFactory& )=delete;
    std::map<Algorithm_order,AlgorithmSolution*>    m_factory;
};
#ifdef __linux__
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<cstring>
#include<errno.h>
#endif
struct GetFileLastModifyTime{
    static time_t  last_file_modify_time(const char* filename )
    {
#ifdef __linux__
        struct stat stat_buf;
        memset(&stat_buf,0x00,sizeof(stat_buf));
        if(stat(filename, &stat_buf) == -1 )
        {
            fprintf(stderr,"stat error %s\n",strerror(errno));
            return 0;
        }
        return stat_buf.st_mtim.tv_sec;
#else
        return 0;//除了linux其他的平台暂不实现
#endif
    }
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
    algorithm_register(const char*filename )
    {
        T * temp = new T();
        TestSolutionFactory::Instant().Regiest( TestSolutionFactory::Algorithm_order(temp->m_index,\
        GetFileLastModifyTime::last_file_modify_time(filename)),temp );
    }
};
template<typename T>
struct algorithm_register_agent{
    static algorithm_register<T> help;
};
//终极解决方案 就是这个 注意 这里需要模板特例化 也就是那个template<>不能少 少了就编译报错了
//而且这个必须得初始化 不初始化则不会调用构造函数 坑爹啊啊啊啊啊啊啊啊啊啊 
// 但是如果不是模板类的静态成员这里就只需要定义而不需要初始化就会调用构造函数不知道为啥
#define RegistAlgorithm(classname) \
 template<> algorithm_register<classname> algorithm_register_agent<classname>::help = algorithm_register<classname>(__FILE__) ;


#define GetOneTest(type)  TestSolutionFactory::Instant().GetOne(type)
#define GetHelp()  TestSolutionFactory::Instant().PrintAll()
#endif