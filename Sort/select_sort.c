#include "select_sort.h"
void select_sort( int*ptr, int len )
{
    int select =0;
    for( int i=0;i<len-1;i++ )
    {
        select = i;
        for( int j=i+1;j<len;j++ )
        {
            if( ptr[j]<ptr[select] )
                select = j;
        }
        if( i == select )
            continue;
        ptr[i] ^=ptr[select];
        ptr[select] ^=ptr[i];
        ptr[i]^=ptr[select];
    }
}
