//#ifdef REDIS_VALUE_TEST
#include "redis_value.hpp"
#include <atomic>
using namespace async_redis_client;
std::atomic<int> construct_size{};
std::atomic<int> destruct_size{};
#include <cstdio>
#include <thread>
#define TrueOrFalse(ret) (ret ? "true" : "false")
struct TestMutipleThread
{
    friend async_redis_client::redis_value;
    void test()
    {
        redis_value empty;
        redis_value str("abccdddd");
        assert(str.is_string());
        redis_value number(100);
        assert(number.is_interger());
        nilTag tag;
        redis_value nil(tag);
        redis_value nil2(nilTag{}); //这里一定得是 nilTag{} 不能是nilTag() 不然是报错了 编译期会错认为 nilTag是个函数指针
        printf("nil2:type:%s\n", nil2.type_name().c_str());
        assert(nil2.is_nil());
        redis_value err(errorTag(), "somestring error ");
        assert(err.is_error());

        //redis_value valarray{str, number, err};
        //assert(valarray.is_array());

        redis_value valarray;
        valarray.push_back(empty);
        valarray.push_back(str);
        valarray.push_back(std::move(number));
        valarray.push_back(nil);
        valarray.push_back(nil2);
        valarray.push_back(std::move(err));

        printf("empty:name_type:%s is_empty:%s\n", empty.type_name().c_str(), TrueOrFalse(empty.is_empty()));
        printf("str::name_type:%s  is_str:%s \n", str.type_name().c_str(), TrueOrFalse(str.is_string()));
        printf("number:name_type:%s is_number:%s\n", number.type_name().c_str(), TrueOrFalse(number.is_interger()));
        printf("nil:name_type:%s is_nil:%s\n", nil.type_name().c_str(), TrueOrFalse(nil.is_nil()));
        printf("err:name_type:%s is_errno:%s\n", err.type_name().c_str(), TrueOrFalse(err.is_error()));
        printf("valarray:name_type:%s is_array:%s\n", valarray.type_name().c_str(), TrueOrFalse(valarray.is_array()));

        // assert(valarray.is_array());
    }
};
void test()
{
    std::vector<std::thread> ths;
    const int threadCounts = 1;
    for (size_t idx = 0; idx < threadCounts; idx++)
    {
        ths.emplace_back([]()
                         {
                             TestMutipleThread t;
                             t.test();
                         });
    }
    for (std::thread &th : ths)
        th.join();
}
int main()
{
    test();
    printf("all test end construct size:%d  destruct size:%d\n", construct_size.load(), destruct_size.load());
}
///#endif