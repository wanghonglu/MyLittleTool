#include "BackTrace.h"
bool SetSingalHandler( int signal, SignalHandlerType handler, int flag  )
{
    struct sigaction sa;
    memset( &sa,0x00,sizeof( struct sigaction ) );
    sigemptyset(&sa.sa_mask);
    if( flag>0  )
        sa.sa_flags = flag;
    sa.sa_handler = handler;
    return  sigaction( signal, &sa, NULL ) ==0;
}
std::function<void(int)>  TraceBackBase::m_cb = nullptr;
std::ofstream  TraceBackBase::m_errorfile;
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

 static std::string addr2line(const char* exe, const char* addr ) {
     int pipefd[2];
     abort_if(pipe(pipefd) != 0, "create pipe failed");
 
     pid_t pid = fork();
     if (pid == 0) {
         close(pipefd[0]);
         dup2(pipefd[1], STDOUT_FILENO);
         dup2(pipefd[1], STDERR_FILENO);
 
       #ifdef __linux__
         int r = execlp("addr2line", "addr2line", addr, "-f", "-C", "-e", exe, (void*)0);
         abort_if(r == -1, "execlp addr2line failed");
       #else
         int r = execlp("atos", "atos", "-o", exe, addr, (void*)0);
         abort_if(r == -1, "execlp atos failed");
       #endif
     }
 
     close(pipefd[1]);
     abort_if(waitpid(pid, NULL, 0) != pid, "waitpid failed");
 
     char buf[4096]={0};
     ssize_t r = read(pipefd[0], buf, 4095);
     close(pipefd[0]);
     buf[r > 0 ? r : 0] = '\0';
	 return std::string( buf, r+1 );
 }
#include<stdio.h>
#include<iostream>
using namespace std;
void UnixTraceBack::SigleHandle(int signal )
{
    fprintf( stderr, "sigal %d\n", signal );
    if( m_cb  )
        m_cb( signal );
    pid_t  pid = fork();
    if( pid >0  )
    {
        /* parent process */
        int status;
        kill( getpid(), SIGSTOP );//父进程暂停
        waitpid( pid, &status, WNOHANG );
        SetSingalHandler(SIGABRT, SIG_DFL, 0);
//        abort();//正常挂掉
              _Exit(EXIT_SUCCESS);
    }
    std::string exe = GetProcessName();
    std::stringstream errmsg;
    do {
        switch (signal) {
          case SIGSEGV:
            errmsg<<"SIGSEGV: segmentation fault\n";
            break;
          case SIGABRT:
            errmsg<<"SIGABRT: aborted\n";
            break;
          case SIGFPE:
            errmsg<<"SIGFPE: floating point exception\n";
            break;
          case SIGBUS:
            errmsg<<"SIGBUS: bus error\n";
            break;
          case SIGILL:
            errmsg<<"SIGILL: illegal instruction\n";
            break;
          default:
            errmsg<<"caught unexpected signal "<< signal <<"\n";
            break;
        }
    } while (0);
#if 0
    void* addrs[4096]={NULL};// last 4k
     int nframes = backtrace(addrs, 128);
     fprintf( stderr, "nframes %d\n", nframes );
//     int nskip = nframes > 2 ? 2 : 0;
     //if (check_failed && nframes > 7) nskip = 7;
 
     int nskip=0;
     int maxaddrlen = 0;
     char* buf = ((char*)addrs) + nframes * sizeof(void*);
     size_t buflen = 4096 - nframes * sizeof(void*);

     std::string stack_info;
     for (int i = nskip; i < nframes; ++i) {
         errmsg << '#' << (i - nskip) << "  ";
 
         Dl_info di;
         if (dladdr(addrs[i], &di) == 0 || di.dli_fname[0] != '/' || exe == di.dli_fname) {
             errmsg << addrs[i]<<"  ";
			 fprintf( stderr, "======%s\n", exe.c_str() );
             stack_info = addr2line(exe.c_str(), (char*)addrs[i]);
         } else {
             void* p = (void*) ((char*)addrs[i] - (char*)di.dli_fbase);
             errmsg << p;
             stack_info = addr2line(di.dli_fname, (char*)p );
         }
 
         errmsg<< " in ";

         auto pos = stack_info.find('\n');
         if( std::string::npos !=pos  )
         {
			 stack_info[pos]='\0';
             errmsg<<( stack_info.substr( 0,pos ));
             errmsg<<" at ";
             if( pos +1 <stack_info.size() )
                 errmsg<<stack_info.substr( pos+1 )<<"\n";
         }
         else
            errmsg<<"???"<<"\n"; 

         
     }
     if( m_errorfile.is_open() )
     {
        m_errorfile<<errmsg.str()<<"\n";
        m_errorfile.close();
     }
     else 
         cerr<<"error1111111"<<endl;
     cerr<<errmsg.str()<<endl;
#endif
     kill(getppid(), SIGCONT);//让父进程继续执行
     _Exit(EXIT_SUCCESS);


}
