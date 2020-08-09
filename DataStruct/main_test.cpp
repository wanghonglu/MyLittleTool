#include "test.hpp"
#include<iostream>
#include<string.h>
#include<map>
using TestFunType= std::map<int,std::pair<std::string,std::function<void(int,char**)>>>;
void PrintHelp(const TestFunType& funs )
{
    std::cout<<"Useage: Bin Opt "<<std::endl;
    for( const auto& e:funs)
    {
        std::cout<<e.first<<" "<<e.second.first<<std::endl;
    }
    std::cout<<std::endl;
}
//#pragma pack(4)
int main( int argc,char**argv )
{
    try{
        int opt=-1;
        TestFunType m_test_function={
            {0,{"栈测试",stack_test}},
            {1,{"堆",priority_queue_test}},
            {2,{"队列",queue_test}},
            {3,{"双向队列",deque_test}},
            {4,{"二叉搜索树",[](int argc,char**argv){binarySearchTree_test(1,argv);}}},
            {5,{"AVL树",[](int argc,char**argv){binarySearchTree_test(2,argv);}}},
            {6,{"红黑树",[](int argc,char**argv){binarySearchTree_test(3,argv);}}}
        };
        if( argc>1 )
            opt = atoi(argv[1]);
        if( !m_test_function.count(opt) )
            PrintHelp(m_test_function);
        else
            m_test_function[opt].second(opt,argv);
		std::cin.get();
    }
    catch( const std::exception& err )
    {
        std::cerr<<" test  error "<<err.what()<<std::endl;
		std::cin.get();
    }
    return 0;
}
