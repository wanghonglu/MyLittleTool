#include "bubble_sort.h"
void
bubble_sort( int*ptr, int len )
{
    for( int i=0;i<len-1;i++ )
        for( int j=0;j<len-i-1;j++ )
        {
            if( ptr[j]>ptr[j+1] )
            {
                ptr[j] ^=ptr[j+1];
                ptr[j+1] ^=ptr[j];
                ptr[j]  ^=ptr[j+1];
            }
        }
}
