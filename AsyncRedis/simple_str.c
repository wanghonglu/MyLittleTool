#include "simple_str.h"

size_t sdsgetsize(sds s )
{
    if(s == NULL ) return 0;
    simple_str* ss = SDS_CAST(s);
    return ss->len_;
}
size_t sdsgetalloc(sds s)
{
    if( s == NULL ) return 0;
    simple_str*ss = SDS_CAST(s);
    return ss->alloc_;    
}
//构造
sds sdsnewlen(const void*src, size_t initlen )
{
    simple_str *ss;

    if( (ss = (simple_str*)malloc( SDS_HEAD_SIZE+initlen+1 ) )== NULL  )
    {
        SDS_ERROR("malloc");
        return NULL;
    }
    ss->len_=initlen;
    ss->alloc_=initlen;

    if( src && initlen )
        memcpy( ss->buf_, src, initlen);
    ss->buf_[initlen]='\0';
    return ss->buf_;
}
sds sdsnewstr( const char*src )
{
    size_t len = src==NULL?0:strlen(src);
    return sdsnewlen( src, len);
}
sds sdsnew(size_t size )
{
    simple_str *ss;
    if( (ss = (simple_str*)malloc( size+SDS_HEAD_SIZE+1 )) == NULL )
    {
        SDS_ERROR("malloc");
        return NULL;
    }
    ss->len_= 0;
    ss->alloc_=size;
    ss->buf_[0]='\0';
    return ss->buf_;
}
sds sdsempty()
{
    return sdsnewlen("",0 );
}
void sdsfree(sds str )
{
    if(str == NULL ) return;
    free( str-SDS_HEAD_SIZE);
}
sds sdsMakeRoom(sds s, size_t len )
{
    if(s == NULL ) return NULL;

    simple_str* ss =(simple_str*)(s-SDS_HEAD_SIZE);
    simple_str* new_ss =  NULL;

    size_t sizefree = ss->alloc_ - ss->len_;
    if( sizefree >= len )
        return s ;
    size_t realsize =0;
    RealSize(ss->len_+len, realsize )

    if ((new_ss = realloc(ss, SDS_HEAD_SIZE+realsize )) == NULL )
    {
        SDS_ERROR("realloc");
        return NULL;
    }
    s = new_ss->buf_;
    new_ss->alloc_= realsize;

    return new_ss->buf_; 
}
sds sdscatlen( sds s, const void*src, size_t len )
{
    if( s == NULL ) return NULL;

    s = sdsMakeRoom(s, len );
    simple_str* ss = SDS_CAST(s);
    
    memcpy( s + ss->len_, src, len );

    ss->len_+=len;
    s[ss->len_]='\0';//最开始就+1 即真正内存比alloc多1 所以这里依然是可以的
    return ss->buf_;
}
sds sdscat(sds s, const char*src )
{
    if(s == NULL )return NULL;
    return sdscatlen(s, src, strlen(src));
}
sds sdscopy( sds src )
{
    if( NULL == src )
        return NULL;
    simple_str* des =(simple_str*)malloc(SDS_HEAD_SIZE+sdsgetalloc(src)+1); 
    if(NULL == des )
    {
        SDS_ERROR("realloc");
        return NULL;
    }

    memcpy( des->buf_, src, sdsgetsize(src));
    des->alloc_ = sdsgetalloc(src);
    des->len_= sdsgetsize(src);
    des->buf_[des->len_]='\0';
    return des->buf_;
}

void sdssetlen(sds s, size_t len )
{
    if( s == NULL ) return;
    simple_str* ss = SDS_CAST(s);

    ss->len_ = len;
}
void sdsclear(sds s)
{
    sdssetlen(s, 0 );    
}

sds sdssprintf( sds s, const char*fmt, ... )
{
    va_list ap;
    char staticbuf[1024+1]={0}, *buf=NULL;
    int n=0, bufsize=1024;

    buf = staticbuf;
    for(;;)
    {
        va_start(ap,fmt);
        n = vsnprintf(buf, bufsize, fmt, ap );
        va_end(ap);
        if(n<0)
            return NULL;
        if( n<bufsize )
            break; 
        bufsize *=2; 
        if( buf == staticbuf )
           buf = (char*)malloc(bufsize);
        else 
            buf = realloc( buf, bufsize); 
    }
    s = sdscat(s, buf);
    if( buf != staticbuf )
        free(buf);
    return s;
}

#ifdef TEST_SDS
void sds_unit_test(int count )
{
   sds s = sdsempty();
   sds s1 = sdsnewstr("abcccc");
   assert( strcmp(s1, "abcccc") == 0 );

   int i=0;
   for(;i<count;i++ )
   {
       s = sdscat(s, "abcc");
    
       sds s3 = sdsnew(100);
       sds s4 = sdscopy(s);
       printf( "s=%s s4=%s strcmp:%d\n", s,s4, strcmp(s, s4) );
       assert( strcmp(s4, s ) ==0 );
       sdsfree(s3);
       sdsfree(s4);
       //printf("%s, %d\n", s, sdsgetalloc(s));
   }

   sdsclear(s1);

   s1 = sdssprintf( s1, "lllllfffffffffgggggggggaaaaaa %s %d ======== %s ", "test", 100, s );

   printf("sdsprintf:%s\n", s1 );


   sdsfree(s);
   sdsfree(s1);

   fprintf(stderr, "test end\n ");

}
int main( int argc, char**argv )
{
    if( argc<2 )
    {
        fprintf( stderr, "Userge bin count\n");
        return 0;
    }
    size_t count = atoi( argv[1] );
    sds_unit_test(count);
    return 0;
}
#endif










