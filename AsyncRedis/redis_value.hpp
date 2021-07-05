/*
    redis value的设计参考 nlohmann::json的核心结构
    即用一个 union 但是c++ 并不支持针对对象的union 比如string 比如vector之类的
    用了指针
*/
#include <cstdint>
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <cassert>
#ifdef REDIS_VALUE_TEST
#include <atomic>
extern std::atomic<int> construct_size;
extern std::atomic<int> destruct_size;
#endif
namespace async_redis_client
{
    template <template <typename U> class Alloc>
    class redis_value_t;
    struct errorTag
    {
    };
    struct nilTag
    {
    };
    template <template <typename U> class Alloc = std::allocator>
    class redis_value_t
    {
        enum RedisValueType
        {
            T_Empty = 0,
            T_NIL,
            T_Interger,
            T_Error,
            T_String,
            T_Array,
        };
        union RedisValue
        {
            int64_t number_interger_;
            std::string *str_;
            std::string *err_message_;
            std::vector<redis_value_t, Alloc<redis_value_t>> *array_;
        };

    public:
        using Array_t = std::vector<redis_value_t, Alloc<redis_value_t>>;
        template <typename T, typename... Args>
        static T *create_obj(Args &&...args)
        {
            Alloc<T> alloc;
            T *obj = alloc.allocate(1);
            alloc.construct(obj, std::forward<Args>(args)...);
#ifdef REDIS_VALUE_TEST
            construct_size++;
#endif
            return obj;
        }
        template <typename T>
        static void destroy_obj(T *obj)
        {
            Alloc<T> alloc;
            alloc.destroy(obj);
            alloc.deallocate(obj, 1);
#ifdef REDIS_VALUE_TEST
            destruct_size++;
#endif
        }

        redis_value_t() : value_type_(T_Empty)
        {
        }
        template <typename T>
        redis_value_t(T number,
                      typename std::enable_if<std::is_integral<typename std::decay<T>::type>::value, void *>::type ign = nullptr) : value_type_(T_Interger)
        {
            value_.number_interger_ = number;
        }
        //构造string
        redis_value_t(const std::string &str) noexcept
        {
            value_type_ = T_String;
            value_.str_ = create_obj<std::string>(str);
        }
        redis_value_t(const char *str)
        {
            value_type_ = T_String;
            value_.str_ = create_obj<std::string>(str);
        }
        redis_value_t(std::string &&str)
        {
            value_type_ = T_String;
            value_.str_ = create_obj<std::string>(std::move(str));
        }
        redis_value_t(const char *str, size_t len)
        {
            value_type_ = T_String;
            value_.str_ = create_obj<std::string>(str, len);
        }
        //构造数组结构
        redis_value_t(const std::initializer_list<redis_value_t> &init)
        {
            value_type_ = T_Array;
            value_.array_ = create_obj<Array_t>(init);
        }
        //构造错误消息
        redis_value_t(errorTag, const char *str, size_t len)
        {
            value_type_ = T_Error;
            value_.err_message_ = create_obj<std::string>(str, len);
        }
        redis_value_t(errorTag, const std::string &str)
        {
            value_type_ = T_Error;
            value_.err_message_ = create_obj<std::string>(str);
        }
        redis_value_t(errorTag, const char *str)
        {
            value_type_ = T_Error;
            value_.err_message_ = create_obj<std::string>(str);
        }
        redis_value_t(errorTag, std::string &&str)
        {
            value_type_ = T_Error;
            value_.err_message_ = create_obj<std::string>(std::move(str));
        }
        //构造nil 结构
        redis_value_t(nilTag)
        {
            value_type_ = T_NIL;
        }
        //析构函数
        ~redis_value_t()
        {
            switch (value_type_)
            {
            case T_String:
                destroy_obj<std::string>(value_.str_);
                break;
            case T_Error:
                destroy_obj<std::string>(value_.err_message_);
                break;
            case T_Array:
                destroy_obj<Array_t>(value_.array_);
                break;
            default:
                break;
            }
            value_type_ = T_Empty;
        }
        void swap(redis_value_t &other)
        {
            std::swap(value_type_, other.value_type_);
            std::swap(value_, other.value_);
        }
        //拷贝构造 拷贝赋值
        redis_value_t(const redis_value_t &other)
        {
            value_type_ = other.value_type_;
            switch (value_type_)
            {
            case T_Interger:
                value_.number_interger_ = other.value_.number_interger_;
                break;
            case T_Error:
                value_.err_message_ = create_obj<std::string>(*other.value_.err_message_);
                break;
            case T_String:
                value_.str_ = create_obj<std::string>(*other.value_.str_);
                break;
            case T_Array:
                value_.array_ = create_obj<Array_t>(*other.value_.array_);
                break;
            default:
                break;
            }
        }
        redis_value_t &operator=(const redis_value_t &other)
        {
            ~redis_value_t();
            value_type_ = other.value_type_;
            switch (value_type_)
            {
            case T_Interger:
                value_.number_interger_ = other.number_interger_;
                break;
            case T_Error:
                value_.err_message_ = create_obj<std::string>(*other.value_.err_message_);
                break;
            case T_String:
                value_.str_ = create_obj<std::string>(*other.value_.str_);
                break;
            case T_Array:
                value_.array_ = create_obj<Array_t>(*other.value_.array_);
                break;
            default:
                break;
            }
        }
        //移动构造 移动赋值
        redis_value_t(redis_value_t &&other)
        {
            value_type_ = other.value_type_;
            value_ = other.value_;
            other.value_type_ = T_Empty;
            other.value_ = {};
        }
        redis_value_t &operator=(redis_value_t &&other)
        {
            swap(other);
        }
        //以下接口是正常使用的
        bool is_empty() const
        {
            return value_type_ == T_Empty;
        }
        bool is_interger() const
        {
            return value_type_ == T_Interger;
        }
        bool is_string() const
        {
            return value_type_ == T_String;
        }
        bool is_array() const
        {
            return value_type_ == T_Array;
        }
        bool is_error() const
        {
            return value_type_ == T_Error;
        }
        bool is_nil() const
        {
            return value_type_ == T_NIL;
        }
        int64_t ToInterger() const
        {
            if (is_interger())
                return value_.number_interger_;
            return 0;
        }
        const std::string &ToString() const
        {
            if (is_string())
                return *value_.str_;
            static std::string default_string;
            return default_string;
        }
        const Array_t &ToArray() const
        {
            if (is_array())
                return *value_.array_;
            static Array_t default_array;
            return default_array;
        }
        const std::string &ToErrMsg() const
        {
            if (is_error())
                return *value_.err_message_;
            static std::string default_errMsg("No Error");
            return default_errMsg;
        }

    public:
        //push_back /emplace_back 只针对于empty类型 或者array类型
        void push_back(const redis_value_t &obj)
        {
            assert(value_type_ == T_Array || value_type_ == T_Empty);

            if (value_type_ != T_Array && value_type_ != T_Empty)
                throw std::runtime_error("push back error type error" + type_name());

            if (T_Empty == value_type_)
            {
                value_type_ = T_Array;
                value_.array_ = create_obj<Array_t>();
            }
            value_.array_->push_back(obj);
        }
        void push_back(redis_value_t &&obj)
        {
            assert(value_type_ == T_Array || value_type_ == T_Empty);

            if (value_type_ != T_Array && value_type_ != T_Empty)
                throw std::runtime_error("push back error type error" + type_name());

            if (T_Empty == value_type_)
            {
                value_type_ = T_Array;
                value_.array_ = create_obj<Array_t>();
            }
            value_.array_->emplace_back(std::move(obj));
        }

    public:
        std::string type_name()
        {
            switch (value_type_)
            {
            case T_Empty:
                return "empty_type";
            case T_NIL:
                return "nil type";
            case T_Interger:
                return "Number type";
            case T_Error:
                return "error type";
            case T_String:
                return "string type";
            case T_Array:
                return "array type";
            default:
                return "unknown type";
            }
        }

    private:
        RedisValueType value_type_ = T_Empty;
        RedisValue value_;
    };
    using redis_value = redis_value_t<>;
}
