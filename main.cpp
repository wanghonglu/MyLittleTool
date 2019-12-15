#include "def.h"
#ifdef LRU_TEST
#include "LRU_Cache.h"
#endif
#include "log.h"
using namespace std;
#include "json.hpp"
#include "Linux_StartProcess.hpp"
#include "ConsulClient.h"
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
    SLOG_DEBUG("test");
    SLOG_ERROR("AGDD"<<9000<<"  FASDFSD");
    SLOG_ERROR( 100 );

    JLOG_INFO.Num(109).Message("info","this is a test ").Msg("ceshi ");

    //consul test
    gConsul.Init( "192.168.199.233", 8500 );
    RegistConsul("LearnToConsul/SimpleConfig/this_is_a_test", ConfigTest );

    gLog.Join();
    while(1)
        ;
}
