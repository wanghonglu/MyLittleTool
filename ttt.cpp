#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include<bitset>
using namespace std;
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
    return 0;
}

