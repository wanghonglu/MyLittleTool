#include "ThreadSigleton.hpp"
#include<thread>
#include<iostream>
using namespace std;
int& func()
{
    static thread_local int number2 =0;
    return number2;
}
int main()
{
   std::thread t1([](){
       int& number = Singleton<int>::Instance();
       number++;

       ++func();
       cout<<func()<<endl;
       cout<<Singleton<int>::Instance()<<endl; 
       }); 
    t1.join();
    int& number = Singleton<int>::Instance();
    cout<<Singleton<int>::Instance()<<endl; 
       cout<<func()<<endl;

    return 0;
}
