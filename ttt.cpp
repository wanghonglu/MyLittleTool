#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include<bitset>
#include<functional>
#include<vector>
#include<memory>
using namespace std;
struct Test:public std::enable_shared_from_this<Test>{
    void PutFunction( std::vector<std::function<void(void)>>& funcs )
    {
        funcs.push_back([ptr = shared_from_this()](){ptr->Dosometings();});
    }
    std::shared_ptr<Test> GetShared()
    {
        return std::shared_ptr<Test>(this);
    }
    void Dosometings()
    {
        std::cout<<"do sometings"<<a<<std::endl;
    }
    ~Test()
    {
        std::cout<<"destruct"<<std::endl;
    }
    int a=100;
};
double getRandom()
{
    static int init=0;
    if( !init  )
    {
        srand(time(NULL));
        init = 1;
    }
    int temp = rand()%1000;
    return (double)temp/1000;
}
double getPai(int counts)
{
    int incircle=0;
    double x,y;
    for( int i=0;i<counts;++i  )
    {
        x = getRandom();
        y = getRandom();
        if(  (pow(x,2)+pow(y,2)) <1 )
            ++incircle;
    }
    return 4*(double)incircle/counts;
}
#define Printf( p,n  ) do{\
if( p&n  ) printf("1");\
else printf("0");\
}while(0)
template<typename T>
void PrintBianary( T n  )
{
    cout<<n<<" 类型 "<<typeid(T).name()<<endl;
    unsigned char *ptr = new unsigned char[sizeof(T)];

    ptr = reinterpret_cast<unsigned char*>(&n);
    //小端
    for( int i=sizeof(T)-1;i>=0;--i )
    {
        //这里还得倒过来 小端 低位才是低字节 
        //unsigned char t = (ptr[i]<<4) | (ptr[i]>>4);
        unsigned char t = ptr[i];
        if( !t )
            printf("00000000");
        else
        {
            for( int j=7;j>=0;j-- )
                Printf(t,1<<j);
        }
        printf(" ");
    }
    printf("\n");
}
int main( int argc,char**argv  )
{
    if( argc>1  )
        printf("pai = %lf\n", getPai( atoi(argv[1] ) ) );
    double a=7.1;

    PrintBianary(a);
    int b=-5;
    PrintBianary(b);
    float c=125.5;
    PrintBianary(c);
    float d=7.1;
    std::bitset<32> c23(d);
    cout<<c23<<endl;
    PrintBianary(d);
    short e=1156;
    PrintBianary(e);


    double e1=0.100000002333;
    double e2=0.12223330000000000000000000000000000000000000000000000000000000000001;
    printf( "%.20lf\n", e1+e2 );
    cout<<e1+e2<<endl;

    std::vector<std::function<void(void)>>func;
    {
        std::shared_ptr<Test> t( new Test());
        t->PutFunction(func);
    }
    int aaaa=10;
    int bbbb=100;
    if(!func.empty())
        func[0]();

    return 0;
}

