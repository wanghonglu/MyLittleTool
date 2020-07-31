#include "test.hpp"
#include<iostream>
#include<map>
//#pragma pack(4)
struct A {
	char a;
	long long b;
	char d;
};
struct B {
	char a;
	unsigned short b;
};
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
		//binarySearchTree_test(1,argv);
		//AVL树
	/*	binarySearchTree_test(2, argv);
		std::map<int, int> p;*/
		std::cout << sizeof(void*) << std::endl;
		std::cout << sizeof(long long ) << std::endl;
		std::cout << sizeof(struct A) << std::endl;
		std::cout << sizeof(struct B) << std::endl;
		std::cout <<(int) &(((struct A*)0)->a) << std::endl;
		std::cout << (int)&(((struct A*)0)->b) << std::endl;
		std::cout << (int) &(((struct B*)0)->a) << std::endl;
		std::cout << (int)&(((struct B*)0)->b) << std::endl;
		system("pause");
    }
    catch( const std::exception& err )
    {
        std::cerr<<" test  error "<<err.what()<<std::endl;
		system("pause");
    }
    return 0;
}
