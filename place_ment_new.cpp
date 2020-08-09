#include<new>
#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;
class T{
    public:
    T():number(100),str("aaa"){
        cout<<"construct T"<<endl;
    }
    ~T()
    {
        cout<<"destruct"<<endl;
    }
    int number;
    string str;
};
int main()
{
    char* ptr = new char[100];

    T *t = new(ptr)T();//定位构造 需要包含new

    cout<<t->number<<endl;

    int a = *reinterpret_cast<int*>(ptr);
    cout<<a<<endl;

    //这里必须手动调用析构函数
    t->~T();

    //delete t;
    delete ptr;
    //cout<<t->number<<endl; //不能这么做 因为内存已经释放了
    //
    T * t2 = new T();

    free( t2 );//这样也没问题 对象本身的内存可以释放,但是析构函数不会调起

}
