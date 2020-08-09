 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>
 #define MAX(x,y) ((x)>(y)?(x):(y))
 #define MAX2(x,y) ({\
     typeof(x) _x=(x);\
     typeof(y) _y=(y);\
     _x>_y?_x:_y;\
 })
 #define MAX3(x,y) ({\
     typeof(x) _x=(x);\
     typeof(y) _y=(y);\
     (void)(&_x==&_y);\
     _x>_y?_x:_y;\
 })
 int main()
 {
     int a=10;
     int  b=9;
     printf( "%d\n" ,MAX(a,b) );
     //printf( "%d\n" ,MAX(--a,++b) );
     //printf( "%d\n" ,MAX2(--a,++b) );
     printf( "%d\n" ,MAX3(--a,++b) );
     short c=-5;
     double cc = c;
     printf("sizeof(int):%ld sizeof(double):%ld %lf\n", sizeof(int),sizeof(double),cc);
 }

