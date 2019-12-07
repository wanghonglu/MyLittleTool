/*  Linux下开发程序,部署一些不要的执行函数
 *  1 设置core文件 当进程崩溃的时候默认是不产生core文件的，这个是必须的
 *  2 设置栈大小   一般的进程默认是10M的栈大小，显然不适合大部分服务器程序
 *  3 设置进程文件描述符限制  一些网关类似的服务，可能需要有很多socket链接
 *  4 设置后台运行  这个很重要 一般的程序都不是后台运行 需要这个来设置
 *      而且由bash启动的程序，当bash关闭后，进程会结束，需要将运行进程设置为
 *      新的会话组的主会话
 *      另外 代码里假如有 printf fprintf( stderr  ... )之类的调用，我们希望
 *      将stdout 和stderr重新定向到别的文件中，或者是将其定向到 /dev/null中
 *      即不输出
 */
#ifndef __LINUX_STARTPRO__
#define __LINUX_STARTPRO__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
 /* 设置core文件 */
void SetCoreFileUnlimit()
{
    struct rlimit curLimit,newLimit;
    if( getrlimit(RLIMIT_CORE,&curLimit)==0 )
    {
        newLimit.rlim_cur = newLimit.rlim_max=RLIM_INFINITY; 
        if( setrlimit(RLIMIT_CORE, &newLimit) != 0 )
        {
           newLimit.rlim_cur = newLimit.rlim_max = curLimit.rlim_max;
           setrlimit( RLIMIT_CORE, &newLimit );
        }
    }

}
//socket 最大限制
void SetSocketFileSize(size_t  size=0 )
{
    struct rlimit curLimit,newLimit;
    if( getrlimit( RLIMIT_NOFILE,&curLimit ) ==0 )
    {
        newLimit.rlim_cur = newLimit.rlim_max = size>0?size:RLIM_INFINITY;
        if( setrlimit( RLIMIT_NOFILE, &newLimit )!=0 )
        {
            newLimit.rlim_cur = newLimit.rlim_max = curLimit.rlim_max;
            setrlimit(RLIMIT_NOFILE, &newLimit);
        }
    }
}
// 修改栈空间
// 切记在当前进程内修改是不会生效的，只有修改后 fork一个子进程，在子进程里面才会生效
void SetProcessStackSize( size_t size=0  )
{
    struct rlimit curLimit,newLimit;
    if( getrlimit( RLIMIT_STACK,&curLimit ) ==0 )
    {
        newLimit.rlim_cur = newLimit.rlim_max = size>0?size:RLIM_INFINITY;
        if( setrlimit( RLIMIT_STACK, &newLimit )!=0 )
        {
            newLimit.rlim_cur = newLimit.rlim_max = curLimit.rlim_max;
            setrlimit(RLIMIT_NOFILE, &newLimit);
        }
    }

}
/* 修改当前进程为新的会话组的主会话进程，这样可以保证即使bash进程结束，进程还会运行
 */
void Daemonize( const char* dir  )
{
    if( 0!=fork() )
        exit(0);//父进程结束
    if( -1 ==setsid()  )
        exit(0);//将该进程脱离原会话组，新建新的会话，并让该进程为会话主进程
    if( 0!=fork() )
        exit(0);//这个感觉没必要
    chdir(dir);
}
int SwitchFileTo( int fd ,const char*file )
{
    int tmpfd;
    close( fd );

    //如果是重新定位到另一个文件 这里面的标志位必须有 CREAT(创建) APPEND(原文件的基础上追加)
    if( strcmp( file, "/dev/null" ) !=0 )
        tmpfd = open(file,O_RDWR|O_CREAT|O_APPEND, 0644);//拥有这 读写权限 同组用户读 其他用户读权限
    else 
        tmpfd = open( file,O_RDWR );
    if( tmpfd !=-1 && tmpfd !=fd  )
    {
        // dup2( oldfd, newfd );
        //dup2与dup区别是dup2可以用参数newfd指定新文件描述符的数值。若参数newfd已经被程序使用，则系统就会将newfd所指的文件关闭，若newfd等于oldfd，
        dup2( tmpfd, fd );
        close( tmpfd ); //这时候 fd实际指向的就是tmpfd
    }
    return tmpfd !=-1?0:-1;
}
// 重新定向 stdout stdin stderr这三个文件描述符
// 可以定向到 dev/null 也可以定向到一个新建文件夹
// 以前的公司会把 stderr和stdout定向到当前目录的某个文件 
// 估计是写了很多报错啥的吧 把这个入口留着
void SetStdFileDescription( const char*stdoutFile, const char* stderrFile )
{
    SwitchFileTo(STDIN_FILENO, "dev/null");
    SwitchFileTo(STDOUT_FILENO, stdoutFile );
    SwitchFileTo(STDERR_FILENO, stderrFile );
}
void BeforStartProcess( const char*stdoutFile="dev/null", const char*stderrFile="dev/null" )
{
    char curPath[1024]={0};
    getcwd(curPath,1024);
    chdir(curPath);

    SetCoreFileUnlimit();

    SetSocketFileSize();

    SetProcessStackSize();

    Daemonize(curPath);
    chdir(curPath);

    SetStdFileDescription( stdoutFile, stderrFile );

}

#endif
