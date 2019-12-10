#include "def.h"
#ifdef LRU_TEST
#include "LRU_Cache.h"
#endif
#include "log.h"
using namespace std;
#include "json.hpp"
#include "Linux_StartProcess.hpp"
#include "BackTrace.h"
int main( int argc,char**argv )
{
#ifdef LRU_TEST
    LRU_Test();
#endif
    std::unique_ptr<TraceBackBase> g_traceBack=nullptr ;
#ifdef __linux__
    g_traceBack.reset( new UnixTraceBack() );
#endif
    if(g_traceBack)
        g_traceBack->RegistSighandler();
  //  BeforStartProcess( "./stdout", "./stderr" );
    cerr<<GetRandomNumber()<<endl;
    cout<<GetRandomString()<<endl;
    nlohmann::json j;
    j["abc"]=100;
    cout<<j.dump(2)<<endl;


    gLog.Init( "first" );
    gLog.Start();
    SLOG_DEBUG("test");
    SLOG_ERROR("AGDD"<<9000<<"  FASDFSD");
    SLOG_ERROR( 100 );

    gLog.Join();
    while(1)
      ;
}
