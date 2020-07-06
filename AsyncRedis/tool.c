#include "tool.h"
uint64_t
now_micsec()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec*1e6+now.tv_usec;
}

uint64_t
now_sec()
{
    return now_micsec()/1e6;
}
uint64_t
now_millisec()
{
    return now_micsec()/1e3;
}
