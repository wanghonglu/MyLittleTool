#include "logger.h"
int main()
{
    octans::utils::SysLog::Inst().Init("test", "./",0,false);
    return 0;
}
