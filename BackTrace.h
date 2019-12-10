/* 再wsl下调试 发现无论是通过ulimit -c更改 core文件限制 还是通过系统api  getrlimit setrlimit 都不能正常产生core文件 */
/* 再/proc/sys/下有没有coredump的相关设置 估计是wsl暂时不支持这种？*/
/* 没办法 只能通过捕获那些信号 挂掉的时候发的信号 然后捕捉到后打印出程序当前运行的堆栈信息*/
/* 这个调试是作用在 在捕捉到这些信号后，fork出一个新的进程，然后给主进程发送SIGSTOP 进程暂停*/
/* 子进程打印出堆栈信息后，再发送SIGCONT*/
/* 当然这个时候 主进程会 DEL掉 ABORT信号 然后abort 保证主进程挂掉 */

#ifndef __BACKTRACE__
#define __BACKTRACE__
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include <sys/wait.h>
#include<string>
#include<fstream>
#include<functional>
#include<cstring>
#include "Singleton.hpp"
#include "def.h"
#include <dlfcn.h>
#define safe_abort(n) \
    do { \
       kill(getppid(), SIGCONT); \
       SetSingalHandler(SIGABRT, SIG_DFL, 0); \
       abort(); \
    } while (n)
#define abort_if(cond, msg) \
   if (cond) { \
      fwrite(msg, 1, strlen(msg), stderr); \
      safe_abort(0); \
   }

/* 考虑到以后可能会实现非linux的版本 这里暂时用基类实现 */
using SignalHandlerType = void(*)(int);
bool    SetSingalHandler( int sign,  SignalHandlerType, int flag);
class TraceBackBase{
    public:
    virtual void RegistSighandler()=0;
    void SetCallBack( std::function<void(int)> );
    static std::function<void(int signal )>  m_cb;
    static std::ofstream                     m_errorfile;
};

class UnixTraceBack:public TraceBackBase{
    public:
    UnixTraceBack()=default;
    ~UnixTraceBack();
    virtual void RegistSighandler()override;
    static  void SigleHandle(int flag);//静态成员函数才能跟函数指针相互转化
};
#endif
