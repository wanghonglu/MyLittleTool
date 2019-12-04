#include "def.h"
#ifdef LRU_TEST
#include "LRU_Cache.h"
#endif
using namespace std;
int main( int argc,char**argv )
{
#ifdef LRU_TEST
    LRU_Test();
#endif
    cout<<GetRandomNumber()<<endl;
    cout<<GetRandomString()<<endl;
}
