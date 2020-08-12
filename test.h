#include<stdio.h>
class Test{
    public:
    Test()
    {
        printf("construct ");
    }
    ~Test()
    {
        printf( "destruct" );
    }
};
