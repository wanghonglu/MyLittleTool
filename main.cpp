#include "def.h"
#ifdef LRU_TEST
#include "LRU_Cache.h"
#endif
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
    while(1)
        ;
}
