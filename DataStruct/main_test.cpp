#include "test.hpp"
#include<iostream>
int main( int argc,char**argv )
{
    try{
        //栈
        //stack_test(argc,argv);
        //堆
        //priority_queue_test(argc,argv);
        //队列
        //queue_test( argc,argv );
        //双向队列
        //deque_test(argc,argv);
		//二叉搜索树
		binarySearchTree_test(argc,argv);


		system("pause");
    }
    catch( const std::exception& err )
    {
        std::cerr<<" test  error "<<err.what()<<std::endl;
		system("pause");
    }
    return 0;
}
