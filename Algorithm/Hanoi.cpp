#include<iostream>
using namespace std;
#include<cstring>
/*  汉诺塔问题:
 *      有三个柱子 A B C A柱子上有碟子,从小到大排序好的,现在要求把 A 柱子上的碟子放到 C 上
 *       一次只能移动一个盘子
 *       大盘子不能压在小盘子上
 *  这个问题乍一看很复杂
 *  有一个盘子  直接拿过去 n=1
 *  有两个盘子  先把最上面的盘子由A->B  然后把大盘子A->C 然后把B上的盘子 B->C即可 n=3
 *  有三个盘子  先把最上面的盘子A->C 然后把次大盘子 A->B 然后把C上的盘子拿到B C->B 然后把A上最大的拿到C A->C
 *  然后把 B上的盘子拿到A B->A 然后把 B->C; A->C  n=7
 *  ....
 *  后面的就不太好分析了 但是可以看出 n=2^n -1
 *  这个问题 如果从微观分析 比较困难,但是宏观上看 就比较简单了
 *  假设有n个盘子,那么移动最大的那个盘子的时候 A上剩下最大的盘子N  B上是从最小到N-1 C上没盘子
 *
 *  即到这一步经过了 把A上N-1个盘子通过C挪动到B的过程  A->(C)->B  
 *  然后把 A上最大的盘子放到C 这是一步
 *  然后把 B上N-1个盘子挪动到通过A挪动到C  B->(A)->C 
 *  可以分析得出 第一步 和第三部是一样的 即N-1个盘子由A挪动到B  和由B挪动到C是一样的 所以f(n)=f(n-1)+1+f(n-1)=2f(n-1)+1;
 *  很明显 f(2)=2f(1)+1=3
 *         f(3)=2f(2)+1=7
 *         ....
 *  然后就能给出简单的递归
 *  fun(n,a,b,c )
 *  {
 *      if( n == 1)
 *         a 上的碟子放到C 
 *      else
 *      {
 *          fun( n-1,a,c,b ) a上的碟子n-1由a放到b上 通过c
 *          a 上的碟子放到C
 *          fun(n-1,b,a,c )  b上的碟子n-1由b放到c上 通过a
 *      }
 *  }
 *  至于c上的最下的碟子不关系,因为它不影响存放
 *
 *  因为n=1时下面的递归也成立 可以统一写成 递归的形式
 *   if(n>0)
 *   {
 *      fun( n-1,a,c,b ) a上的碟子n-1由a放到b上 通过c
 *      a 上的碟子放到C
 *      fun(n-1,b,a,c )  b上的碟子n-1由b放到c上 通过a
 *   }
 *
 *  * */
int count=0;
void towersofHanoi(int n,char a,char b,char c )
{
    if( n>0 )
    {
        towersofHanoi(n-1,a,c,b);
//        cout<<n<<" move top disk for tower ["<<a<<"] to top of tower ["<<c<<"]"<<endl;
        count++;
        towersofHanoi(n-1,b,a, c );
    }
}

