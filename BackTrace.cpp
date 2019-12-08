#include "BackTrace.h"
void TraceBackBase::SetCallBack(std::function<void(int)> cb )
{
	m_cb = cb;
}
UnixTraceBack::~UnixTraceBack()
{
     SetSingalHandler(SIGSEGV, SIG_DFL, 0);
     SetSingalHandler(SIGABRT, SIG_DFL, 0);
     SetSingalHandler(SIGFPE, SIG_DFL, 0);
     SetSingalHandler(SIGBUS, SIG_DFL, 0);
     SetSingalHandler(SIGILL, SIG_DFL, 0);
}
void UnixTraceBack::RegistSighandler()
{
    const int flag = SA_RESTART | SA_ONSTACK;
    SetSingalHandler(SIGSEGV,&UnixTraceBack::SigleHandle, flag);
    SetSingalHandler(SIGABRT,&UnixTraceBack::SigleHandle, flag);
    SetSingalHandler(SIGFPE, &UnixTraceBack::SigleHandle, flag);
    SetSingalHandler(SIGBUS, &UnixTraceBack::SigleHandle, flag);
    SetSingalHandler(SIGILL, &UnixTraceBack::SigleHandle, flag);
    SetSingalHandler(SIGPIPE, SIG_IGN, 0); 

}
bool UnixTraceBack::SetSingalHandler( int signal, SignalHandlerType handler, int flag  )
{
    struct sigaction sa;
    memset( &sa,0x00,sizeof( struct sigaction ) );
    sigemptyset(&sa.sa_mask);
    if( flag>0  )
        sa.sa_flags = flag;
    sa.sa_handler = handler;
    return  sigaction( signal, &sa, NULL ) ==0;
}
void UnixTraceBack::SigleHandle(int signal )
{
    if( m_cb  )
        m_cb( signal );
    pid_t  pid = fork();
    if( pid >0  )
    {
        /* parent process */
        int status;
        kill( getpid(), SIGSTOP );//父进程暂停
        waitpid( pid, &status, WNOHANG );
    }

}
