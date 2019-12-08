#include "def.h"
#ifdef LRU_TEST
#include "LRU_Cache.h"
#endif
#include "log.hpp"
using namespace std;
#include "json.hpp"
#include "Linux_StartProcess.hpp"
int main( int argc,char**argv )
{
#ifdef LRU_TEST
    LRU_Test();
#endif
    BeforStartProcess( "./stdout", "./stderr" );
    cerr<<GetRandomNumber()<<endl;
    cout<<GetRandomString()<<endl;
    nlohmann::json j;
    j["abc"]=100;
    cout<<j.dump(2)<<endl;


    gLog.Init( "first" );
    gLog.Start();
    gLog.Sleep(1000);
    while(1)
        ;
    SLOG_DEBUG("test");
    SLOG_ERROR("AGDD"<<9000<<"  FASDFSD");
    SLOG_ERROR( 100 );

    gLog.Join();
    while(1)
      ;
}
