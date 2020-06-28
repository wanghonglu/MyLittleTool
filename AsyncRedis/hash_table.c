#include "hash_table.h"
hash_table*
hash_table_construct( HashTypeFunction* hash_functions )
{
    hash_table* table =(hash_table*)malloc(sizeof( hash_table ));
    if( NULL == table )
    {
        HashError("malloc error " );
        return NULL;
    }
    table->size_ = table->use_ = 0;
    table->hash_functions_ = hash_functions;
    table->table_ = NULL;
    assert( table->hash_functions_->hash_ &&
            table->hash_functions_->comp_);
    return table;
}
table_entry* 
hash_table_find( hash_table* t, void*key )
{
    if( t->table_ == NULL || t->size_ == 0 )
        return NULL;
    uint32_t keyIndex = HashSlot(t, key);

    table_entry *entry = t->table_[keyIndex];

    while(entry)
    {
        if( t->hash_functions_->comp_(key, entry->key_ ) )
            return entry;
        entry = entry->next_;
    }
    return NULL;
}
void hash_table_insert( hash_table*table, void*key, void*val )
{
    //存在就覆盖
    table_entry* en = hash_table_find(table, key );
    if( en != NULL )
    {
       HashSetValue(table, en, val );
       return; 
    }
    hash_table_growup(table, table->use_+1); 
    uint32_t  hash_key = HashSlot(table, key );

    en=(table_entry*)malloc(sizeof(table_entry));
    if(NULL == en )
    {
        HashError("malloc error");
        return;
    }
    HashSetKey(table, en, key );
    HashSetValue(table, en, val );
    en->next_ = table->table_[hash_key];
    table->table_[hash_key] = en;
    table->use_++;
}
void 
hash_table_growup(hash_table* table, size_t size )
{
    size_t capcity = 0;
    if( 0 == table->size_ )
        capcity = 32;
    else
    {
        if( (double)table->use_/table->size_ >=ResizeLimit )
            capcity = table->size_*2;//2倍2倍扩容
        else 
            return;
    }

    table_entry** new_table = (table_entry**)malloc(capcity*sizeof(table_entry*));
    if(new_table == NULL )
    {
        HashError(" realloc error " );
        return;
    }
    memset( new_table, 0x00, capcity*sizeof(table_entry*));
    //重新调整hash表里内容
    size_t idx=0;
    table_entry *et, *next;
    uint32_t  hash_key;
    for( ;idx<table->size_;++idx )
    {
        et = table->table_[idx];
        while(et)
        {
            next = et->next_;
            //这里要用新的size 取余
            hash_key = table->hash_functions_->hash_(et->key_)&(capcity-1);
            et->next_ = new_table[hash_key];
            new_table[hash_key] = et;
            et = next;
        }
    }
    if( table->table_ )
        free( table->table_ );
    table->table_ = new_table;
    table->size_ = capcity;
}
//找不到不报错 直接返回
void hash_table_del(hash_table*table, void*key )
{
    uint32_t hash_key = HashSlot(table, key);

    table_entry *en, *pre_en = NULL;
    en = table->table_[hash_key];
    while( en )
    {
        if( table->hash_functions_->comp_(en->key_, key ) )
            break;
        pre_en = en;
        en = en->next_;
    }
    if( NULL == en ) return;

    if( pre_en !=NULL )
        pre_en->next_ = en->next_;
    else
        table->table_[hash_key] = en->next_;
    table->use_--;
    HashFreeKey(table, en );
    HashFreeVal(table, en );
    free(en);
}
void 
hash_table_clear(hash_table *table )
{
    size_t    idx=0;
    size_t    alreadyDel=0; 
    table_entry *en ,*next;
    for( ;idx<table->size_;idx++)
    {
        en = table->table_[idx];
        while(en)
        {
           next = en->next_;
           HashFreeKey( table, en );
           HashFreeVal( table, en );
           free(en);
           ++alreadyDel;
           en = next;
        }
    }
    assert( alreadyDel == table->use_);
    free(table);
}
hash_table_iterator*
hash_table_get_iterator(hash_table* table )
{
    hash_table_iterator* it = (hash_table_iterator*)malloc(sizeof(hash_table_iterator ));
    if( NULL == it )
    {
        HashError(" iterator malloc error " );
        return NULL;
    }
    it->entry_ = NULL;
    it->table_=table;
    it->index_=-1;
}
table_entry*
hash_table_next( hash_table_iterator*it )
{
    while(1)
    {
        if( it->entry_ == NULL )
        {
            it->index_++;
            if(it->index_>=it->table_->size_)
                break;
            it->entry_ = it->table_->table_[it->index_];
        }
        else
        {
            it->entry_ = it->entry_->next_;
        }
        if( it->entry_ !=NULL )
            return it->entry_;
    }
    return NULL;
}
void hash_table_release_iterator(hash_table_iterator*it )
{
    if(it)
        free(it);
}
//key 为socket描述服的hash函数
uint32_t SocketFdHash(const void*key )//摘取自libevent
{

    uint32_t fd = *(uint32_t*)key;
    fd += (fd>>2)|(fd<<30);
    return fd;
}
//key 为int生成hash值 摘取自libevent 
uint32_t IntergerHash(const void*key )
{
   uint32_t h = *(uint32_t*)key;
   h += ~(h << 9);
   h ^=  ((h >> 14) | (h << 18)); /* >>> */
   h +=  (h << 4);
   h ^=  ((h >> 10) | (h << 22)); /* >>> */
   return h;
}
int IntergerCompare(const void*rv, const void*lv )
{
    uint32_t r = *(uint32_t*)rv;
    uint32_t l = *(uint32_t*)lv;
    return r == l;
}
void *IntergerDump(const void*src )
{
    uint32_t* nu = (uint32_t*)malloc(sizeof(uint32_t));
    memcpy(nu, src, sizeof(uint32_t ));
    return nu;
}
//key为string生成hash函数 取自libevent
uint32_t StringHash(const void*key )
{
    uint32_t h;
    const unsigned char*cp = (const unsigned char*)key;
    h = *cp << 7;
    while (*cp) {
      h = (1000003*h) ^ *cp++;
    }
    /* This conversion truncates the length of the string, but that's ok. */
    h ^= (unsigned)(cp-(const unsigned char*)key);
    return h;
}
int StringCompare(const void*rv,const void*lv )
{
    const char*r=(const char*)rv;
    const char*l=(const char*)lv;
    if(strlen(r) != strlen(l) )
        return 0;
    return strcmp(r,l) == 0 ;
}
int StringCaseCompare(const void*rv,const void*lv )
{
    const char*r=(const char*)rv;
    const char*l=(const char*)lv;
    if(strlen(r) != strlen(l) )
        return 0;
    return strcasecmp(r,l) == 0;
}
void SDSFree(void* t)
{
    sds s  = (sds)t;
    sdsfree(s);
}
void* SdsDump(const void*src )
{
    sds s = (sds)src;
    return sdscopy(s);
}
void* StringDump( const void*src )
{
    const char* s = (const char*)src;
    return strdup(s); 
}







#ifdef HASH_TABLE_TEST
static size_t DebugPrintIdx=0;
#define Str2Str 0x03 
#define Num2Num 0x00
#define Num2Str 0x01
#define Str2Num 0x02
#define KeyIsStr(type) (type&0x02)
#define ValIsStr(type) (type&0x01)
#define DebugTable(t) \
fprintf( stderr, "[%d] Line:%d table size:%d Used:%d\n ",++DebugPrintIdx,__LINE__,  t->size_, t->use_);
//打印hash 表每个槽上链表长度占比  看看是否不均匀
#define   VEC_LEN 50
void PrintHashSlotListNum( hash_table* t )
{
    unsigned long slotCount[VEC_LEN+1]={0};
    unsigned max_len=0, idx=0, len, total_chain=0;
    table_entry *en;
    for(;idx<t->size_;++idx )
    {
        en = t->table_[idx];
        if( en == NULL )
        {
            slotCount[0]++;
            continue;
        }
        len =0;
        while(en)
        {
            len++;
            en = en->next_;
        }

        if( len > max_len ) max_len =len;

        slotCount[ len>=VEC_LEN?VEC_LEN:len ]++;
        total_chain+=len;
    }
    fprintf( stderr, "链表内存储元素个数:%d 最长链表:%d 链表总长度:%d\n各个长度链表占比如下\n",
     t->use_, max_len, total_chain );
    for( idx=0;idx<=VEC_LEN;++idx )
    {
        if(slotCount[idx]==0 )
            continue;
        fprintf(stderr, "长度[%d] 占比 %.02lf%% \n", idx, ((double)slotCount[idx]/t->size_)*100 );
    }

}
//打印整个表
void PrintAllTable( hash_table* t, uint8_t Type )
{
    hash_table_iterator* it = hash_table_get_iterator(t);
    table_entry* en = hash_table_next(it);
    while(en)
    {
        switch(Type)
        {
            case Str2Str:
            fprintf( stderr, "Key:%s Value:%s\n", (char*)en->key_, (char*)en->val_ );
            break;
            case Num2Num:
            fprintf( stderr, "Key:%d Value:%d\n", *(int*)en->key_, *(int*)en->val_ );
            break;
            case Num2Str:
            fprintf( stderr, "Key:%d Value:%s\n", *(int*)en->key_, (char*)en->val_ );
            break;
            case Str2Num:
            fprintf( stderr, "Key:%s Value:%d\n", (char*)en->key_, *(int*)en->val_ );
            break;
        }
        en = hash_table_next(it);
    }
    hash_table_release_iterator(it);
}
//检查 查找耗时
void CheckFind(hash_table *t, uint8_t Type )
{
    int num=0;;
    char buf[1024]={0};
    table_entry *en;
    fprintf(stderr, "输入待查找的key\n" );
    scanf("%s", buf );
    uint64_t  t1 = now_micsec();
    if( KeyIsStr(Type)  )
        en = hash_table_find(t, buf );
    else 
    {
        num = atoi(buf);
        en = hash_table_find(t,&num);
    }
    if(en == NULL )
    {
        fprintf(stderr, "找不到key\n" );
        return;
    }
    if(ValIsStr(Type))
        fprintf( stderr, "find Key !! val is [%s] ", (char*)en->val_ );
    else
        fprintf( stderr, "find Key !! val is [%d] ", *(int*)en->val_);

    fprintf( stderr, "cost %d 微秒\n", now_micsec() - t1 );
    return; 
}
//检查 删除
void CheckDelete(hash_table *t, uint8_t Type )
{
    int num=0;;
    char buf[1024]={0};
    fprintf(stderr, "输入待删除的key\n" );
    scanf("%s", buf );
    uint64_t  t1 = now_micsec();
    if( KeyIsStr(Type)  )
        hash_table_del(t, buf );
    else 
    {
        num = atoi(buf);
        hash_table_del(t,&num);
    }
    fprintf( stderr, "删除成功 cost %d 微秒\n", now_micsec() - t1 );
    return; 
}
void CheckTableRight( hash_table* t, uint8_t Type )
{
    fprintf( stderr, "\n\n检查哈希表 \n"
                     "输入n/N结束\n"
                     "输入f/F查找\n"
                     "输入d/D删除\n" );
    char buf[1024]={0};
    int  nu;
    table_entry *en;
    int  end =0;
    while(!end)
    {
        fprintf( stderr, "输入操作\n" );
        scanf("%s", buf );
        switch (buf[0])
        {
            case 'n':
            case 'N':
            end = 1;
            break;
            case 'f':
            case 'F':
            CheckFind(t, Type );
            break;
            case 'd':
            case 'D':
            CheckDelete(t, Type );
            break;
        }
    }
}
void str2str_test(unsigned count )
{
    //string - string table
    hash_table* str2str = hash_table_construct(&StringHashFunctions);
    DebugTable(str2str);
    int i=0;
    hash_table_insert(str2str, "abcdd", "1233" );
    DebugTable(str2str);
    hash_table_insert(str2str, "add", "9999" );
    DebugTable(str2str);
    hash_table_insert(str2str, "abc", "9999" );
    DebugTable(str2str);
    hash_table_insert(str2str, "hhha", "9999" );
    DebugTable(str2str);
    hash_table_insert(str2str, "zhoulllll", "9999" );
    hash_table_insert(str2str, "zhoulllll", "modify" );
    DebugTable(str2str);
    for(;i<count;++i)
    {
        char key[64]={0},value[64]={0};
        snprintf( key,64, "key_%d", i );
        snprintf( value, 64, "value_%d", i );
        hash_table_insert(str2str,key, value );
    }
    //PrintAllTable(str2str, Str2Str );
    PrintHashSlotListNum(str2str);
    table_entry *tmp = hash_table_find(str2str, "abc" );
    if( tmp )
        fprintf(stderr,  "abc value is %s\n", (char*)tmp->val_ );
    table_entry *tmp1 = hash_table_find(str2str, "hhha" );
    if( tmp1 )
        fprintf( stderr, "hhha value is %s\n", (char*)tmp1->val_ );
    DebugTable(str2str);
    CheckTableRight(str2str, Str2Str );
    hash_table_clear(str2str);
}
void int2sds_test(unsigned count )
{
    //interger - sds 
    hash_table* int2sds = hash_table_construct(&IntergeHashFunctions);

    int idx=0;
    sds s1 = sdsnewstr("这是个测试1");
    sds s2 = sdsnewstr("这是个测试2");
    sds s3 = sdsnewstr("这是个测试3");
    sds s4 = sdsnewstr("这是个测试4");
    hash_table_insert(int2sds,&idx,s1 );
    idx++;
    hash_table_insert(int2sds,&idx, s2 );
    idx++;
    hash_table_insert(int2sds,&idx, s3 );
    idx++;
    hash_table_insert(int2sds,&idx, s4 );
    DebugTable(int2sds);

    PrintAllTable(int2sds, Num2Str );

    sdsfree(s1);
    sdsfree(s2);
    sdsfree(s3);
    sdsfree(s4);

    for( ;idx<count;++idx )
    {
        sds tmp  = sdsempty();
        tmp  = sdssprintf(tmp, "this %d 's value ", idx );
        hash_table_insert(int2sds, &idx, tmp );
        sdsfree(tmp);
    }

    PrintHashSlotListNum(int2sds);
    DebugTable(int2sds);
    CheckTableRight(int2sds, Num2Str );
    hash_table_clear(int2sds);



}
void hash_test(unsigned count)
{
    //string - string test
    //str2str_test(count);
    //int -sds test
    int2sds_test(count);
}
int main(int argc,char**argv )
{
    if( argc<2 )
    {
        fprintf( stderr, "Userage bin count \n" );
        return -1;
    }
    unsigned count = atoi(argv[1]);
    hash_test( count);
}
#endif





