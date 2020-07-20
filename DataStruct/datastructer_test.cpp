#include<iostream>
#include<functional>
#include<string>
#include<memory>
#include<map>
#include<random>
#include "test.hpp"
using namespace std;
using namespace datastruct;
void bracket_match();//括号匹配
#define TestSize 59999
#define PrintArray(arry) std::for_each(arry,arry+sizeof(arry)/sizeof(arry[0]), [](decltype(arry[0]) t){std::cout<<t<<" ";});
struct SelfDefine{
    public:
    int number_=0;
    std::string str_;
    public:
    ~SelfDefine()
    {
    }
    SelfDefine(){};
    SelfDefine( int a, const std::string& str ):number_(a),str_(str){};
    friend std::ostream& operator<<(std::ostream& os, const SelfDefine& t );
    bool operator == (const SelfDefine& oth )
    {
        return number_ == oth.number_ && str_ == oth.str_ ;
    }
};
int Random()
{
    static bool init=false;
    if( !init )
    {
        srandom(time(NULL));
        init=true;
    }
    return 1000+random()%1000;
}
std::ostream& operator<<(std::ostream&os, const SelfDefine&t )
{
    os<<"number:"<<t.number_<<" str:"<<t.str_<<std::endl;
    return os;
}
void stack_test( int argc, char**argv  )
{
    if( argc<2)
       return;
    int type = atoi(argv[1]);
    int count =0;
    if( argc>2 )
        count  = atoi(argv[2]);  
    StackBase<int>* ptr = nullptr;
    if( type == 1 )
        ptr = new ArrayStack<int>;
    else
        ptr = new LinkedListStack<int>;
    assert( ptr );
    std::shared_ptr<int> autofree(NULL, [ptr](int*t){ delete ptr;});
    int a[]={1,900,200,300,998};

    if( count <= 0 )
        std::for_each(a, a+sizeof(a)/sizeof(int), [ptr](int v ){ ptr->Push(v);});
    else 
    {
        for( int i=0;i<count;i++ )
            ptr->Push(i);
    }

    std::cout<<(type==1?"ArrayStack test: ":"LinkedListStack test: ")<<std::endl;
    std::cout<<ptr->Size()<<std::endl;
    while( !ptr->IsEmpty() )
    {
        std::cout<<ptr->Top()<<" ";
        ptr->Pop();
    }
    std::cout<<std::endl;

    StackBase<SelfDefine>* tt = nullptr;
    if( type == 1 )
        tt = new ArrayStack<SelfDefine>;
    else
        tt = new LinkedListStack<SelfDefine>;
    std::shared_ptr<int> autofree2(NULL, [tt](int*t){ delete tt;});

    tt->Push( SelfDefine(10,"1" ));
    tt->Push( SelfDefine(11,"2" ));
    tt->Push( SelfDefine(12,"3" ));
    tt->Push( SelfDefine(13,"4" ));

    while( !tt->IsEmpty() )
    {
        std::cout<<tt->Top();
        tt->Pop();
    }
    std::cout<<std::endl;
    std::cout<<"test end "<<std::endl;

    //迭代器测试
    std::shared_ptr<ArrayStack<SelfDefine>> ttt = std::make_shared<ArrayStack<SelfDefine>>();
    for( int i=0;i<40;i++ )
    {
        ttt->Push(SelfDefine(i,"这是第 "+std::to_string(i)+" 个" ));
    }
    for( ArrayStack<SelfDefine>::iterator it = ttt->begin();it!=ttt->end();++it )
        cout<<*it<<endl;
    //bracket_match();
}
//括号匹配问题
void bracket_match()
{
    std::string str;
    static std::map<char,char> matchs={
        {']','['},
        {'}','{'},
        {')','('}
        };
    while( std::cin>>str )
    {
        LinkedListStack<char> st;
        for( int i=0;i<str.size();i++ )
        {
            switch(str[i])
            {
                case '[':
                case '(':
                case '{':
                st.Push(str[i]);
                break;
                case ']':
                case ')':
                case '}':
                if( matchs[str[i]] == st.Top() )
                    st.Pop();
                else
                    std::cout<<"not match"<<endl;
                break;
                default:
                    std::cout<<"not match"<<endl;
                    break;
            }
        }
        if( !st.IsEmpty())
            std::cout<<"not match"<<endl;
        else
            std::cout<<"match"<<endl;
    }
}

void priority_queue_test(int argc, char**argv )
{
    int count =0;
    if( argc>1 )
        count = atoi(argv[1]);

    std::shared_ptr<PriorityQueueBase<int>> ptr;

    int a[]={10,90,2,1,100,1000,456,-2,9000,456};
    if( count == 0 )
    {
        ptr = std::make_shared<ArrayPriorityQueue<int>>( a, sizeof(a)/sizeof(int));
        PrintArray(a); 
        cout<<endl;
    }
    else
    {
        ptr = std::make_shared<ArrayPriorityQueue<int>>();
        for( int i=0;i<count;i++ )
            ptr->push(Random());
    }

    cout<<"ArrayPriorityQueue test: "<<endl;
    std::cout<<std::endl;
    while(!ptr->empty())
    {
        cout<<ptr->top()<<" ";
        ptr->pop();
    }
    std::cout<<endl;
    cout<<__FUNCTION__<<"test end"<<endl;

}
void queue_test( int argc,char**argv )
{
    int type=0;
    if( argc>1 )
        type = atoi(argv[1]);
    std::shared_ptr<QueueBase<std::string>> ptr;
    
    if( type == 0 )
        ptr = std::make_shared<ArrayQueue<std::string>>();
    else
        ptr = std::make_shared<LinkedListQueue<std::string>>();

    ptr->push("aaa");
    assert( ptr->back() == ptr->front() );
    ptr->push("bbb");

    assert( ptr->size() ==2 );

    assert( ptr->back() == "bbb" );
    assert( ptr->front() == "aaa" );

    ptr->pop();
    cout<<ptr->front()<<endl;
    cout<<ptr->back()<<endl;

    std::shared_ptr<QueueBase<SelfDefine>> selfptr;
    if( type == 0 )
        selfptr = std::make_shared<ArrayQueue<SelfDefine>>();
    else
        selfptr = std::make_shared<LinkedListQueue<SelfDefine>>();

    for( int i=0;i<TestSize;i++ )
        selfptr->push( SelfDefine(i, "aaaa" ));

    assert( selfptr->size() ==TestSize );
    assert( selfptr->back().number_ == TestSize-1 );
    assert( selfptr->front().number_ == 0 );
    selfptr->pop();
    assert( selfptr->back().number_ == TestSize-1 );
    assert( selfptr->front().number_ == 1 );

    if( type ==0 )
    {
        cout<<dynamic_cast<ArrayQueue<std::string>*>(ptr.get())->capacity()<<endl;
        cout<<dynamic_cast<ArrayQueue<SelfDefine>*>(selfptr.get())->capacity()<<endl;
    }

    while( ptr->size() || !ptr->empty() )
        ptr->pop();
    while( selfptr->size()||!selfptr->empty())
        selfptr->pop();
    assert( ptr->empty() && ptr->size()==0);
    assert( selfptr->empty() && selfptr->size()==0);
    cout<<__FUNCTION__<<" test end"<<endl;

}
void deque_test(int argc,char**argv )
{
    int type=0;
    if( argc>1 )
        type =1;

    DequeBase<std::string>*  ptr;
    DequeBase<SelfDefine>*   selfptr;
    if( type == 0 )
    {
        ptr = new ArrayDeque<std::string>();
        selfptr = new ArrayDeque<SelfDefine>();
    }
    else 
    {
        ptr = new LinkedListDeque<std::string>();
        selfptr = new LinkedListDeque<SelfDefine>();
    }

    ptr->push_back("abc");

    assert( ptr->front() == ptr->back());

    ptr->pop_front();
    assert( ptr->size()==0 );

    ptr->push_front("aaa");
    assert( ptr->front() == ptr->back());

    ptr->push_back("abc");

    assert(ptr->size()==2 && ptr->back() == "abc" && ptr->front() == "aaa" );

    ptr->pop_back();
    ptr->push_front("eee");
    assert( ptr->back() == "aaa" );

    std::string temp = ptr->front();
    for( int i=0;i<TestSize;i++ )
        ptr->push_back( std::to_string(i) );

    assert( ptr->front() == temp );
    assert( ptr->back() == std::to_string( TestSize-1 ) );

    while( !ptr->empty() )
    {
        ptr->pop_back();
        if( !ptr->empty() )
            ptr->pop_front();
    }

    for( int i=0;i<TestSize;i++ )
        selfptr->push_front( SelfDefine( i, std::to_string(i) ) );

    cout<<selfptr->front()<<endl;
    cout<<selfptr->back()<<endl;
    assert( selfptr->back() == SelfDefine( 0,"0" ) && selfptr->front() == SelfDefine( TestSize-1,std::to_string(TestSize-1) ) );

    assert( selfptr->size() == TestSize );

    while( !selfptr->empty())
    {
        selfptr->pop_front();
        if( !selfptr->empty() )
            selfptr->pop_back();
    }
    assert( selfptr->size()==0 && selfptr->empty() );


    cout<<__FUNCTION__<<" test end"<<endl;
    delete ptr;
    delete selfptr;
}













