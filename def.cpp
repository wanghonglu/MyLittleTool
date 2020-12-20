#include "def.h"
unsigned int Random(unsigned int max,unsigned int min )
{
#ifdef __linux__
    static bool  seed_init = false;
    if( !seed_init  )
    {
        srandom( time(NULL) );
        seed_init=true;
    }
    if( max ==0 ||   max<min )
        return random();
    
    return min+random()%(max-min);
#else
	if (max <= min)
		return GetRandomNumber();
	return min + GetRandomNumber() % (max - min);
#endif

}
int32_t GetRandomNumber()
{
    static std::default_random_engine e(
            std::chrono::system_clock::now().time_since_epoch().count()
            );
    static std::uniform_int_distribution<int32_t>u;

    return u(e);
}
std::string GetRandomString()
{
    static thread_local std::string str="abcdefghijklmnopqrsjuvwxyz@!*&$%+-=;:/.,()[]{}|";
    static std::random_device rd;
    static std::mt19937 engine(rd());

    std::shuffle( str.begin(),str.end(),engine );

    return str;
}
uint64_t now_sec()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}
uint64_t now_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}
uint64_t C_now_sec()
{
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec;
#else 
	return now_sec();
#endif
};
uint64_t C_now_ms()
{
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_usec/1000;
#else
	return now_ms();
#endif
}
std::string DateFmt( char split )
{
	struct tm _tm;
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,NULL);
    localtime_r( &tv.tv_sec, &_tm );
#else //暂时就是windows
	time_t  nows = now_sec();
	localtime_s(&_tm, &nows);
#endif

    std::stringstream ss;
    ss<<_tm.tm_year+1900<<split<<_tm.tm_mon+1<<split<<_tm.tm_mday;
    return ss.str(); 
}
std::string DateTimeFmtBySeconds(uint64_t millisec, char split,bool WithMillSeconds )
{
    struct tm _tm;
    time_t sec = millisec/1000;
#ifdef __linux__
    localtime_r( &sec, &_tm );
#else 
	localtime_s(&_tm, &sec);
#endif
    std::stringstream ss;
    ss<<_tm.tm_year+1900<<split<<_tm.tm_mon+1<<split<<_tm.tm_mday<<" ";
    ss<<_tm.tm_hour<<":"<<_tm.tm_min<<":"<<_tm.tm_sec;
    if( WithMillSeconds )
       ss<<" "<<millisec%1000;
     return ss.str();
}
uint32_t GetBeginOfDay()
{
    struct tm now;
    time_t nowSec = time(NULL);
#ifdef __linux__
    localtime_r( &nowSec, &now  );
#else
	localtime_s(&now, &nowSec);
#endif
    now.tm_hour= now.tm_min = now.tm_sec =0;
    return mktime(&now);
}

inline size_t _thread_id()
{
#ifdef _WIN32
    return  static_cast<size_t>(::GetCurrentThreadId());
#elif __linux__
    return  static_cast<size_t>(syscall(SYS_gettid));
#elif __FreeBSD__
    long tid;
    thr_self(&tid);
    return static_cast<size_t>(tid);
#elif __APPLE__
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<size_t>(tid);
#else //Default to standard C++11 (other Unix)
    return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
}
unsigned int GetThreadId()
{
#if defined(_MSC_VER) && (_MSC_VER < 1900) || defined(__clang__) && !__has_feature(cxx_thread_local)
	return _thread_id();
#else
	static thread_local const size_t tid = _thread_id();
	return tid;
#endif
}
std::string GetProcessName()
{
	std::string _exeName = "/proc/self/exe";
	char exeName[256] = {0};
#ifdef __linux__
	if(readlink("/proc/self/exe" , exeName, sizeof(exeName) ) !=-1 )
	{
		_exeName = exeName;
	}
#endif
	return _exeName;
}

bool			is_str_utf8(const char* str)
{
	//	unsigned int    nWordCount = 0;				 //字符数 不同于字节数
	int	nNeedContinuousByteCount = 0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	unsigned char	UcCurChar = *str;
	for (unsigned int i = 0; str[i] != '\0'; ++i)
	{
		UcCurChar = *(str + i);
		//ASCII用7位编码,最高位标记为0,0xxxxxxx
		//判断是否ASCII编码,如果不是,说明有可能是UTF8 
		//实际表示ASCII字符的UNICODE字符，将会编码成1个字节，并且UTF-8表示与ASCII字符表示是一样的
		if (nNeedContinuousByteCount == 0)   //可以接受带屏蔽位"头字节"了.
		{
			//			++nWordCount;
			//如果不是ASCII码,应该是多字节符,计算字节数			
			//UTF-8编码规则：如果只有一个字节则其最高二进制位为0；
			//如果是多字节，其第一个字节从最高位开始，
			//连续的二进制位值为1的个数决定了其编码的字节数，其余各字节均以10开头。
			if (UcCurChar & 0x80)					//最高位是1
			{
				if ((UcCurChar & 0XFE) == 0XFC)		//0XFE == 11111110   0XFC == 11111100
				{
					//1111110X 6个1 1个0  加紧跟的5个10开头的字节	共6个字节	27~31有效bit位
					nNeedContinuousByteCount = 5;
				}
				else if ((UcCurChar & 0XFC) == 0XF8) //0XFC == 11111100  0XF8 == 11111000
				{
					//111110XX  5个1 1个0  加紧跟的4个10开头的字节	共5个字节	22~26有效bit位
					nNeedContinuousByteCount = 4;
				}
				else if ((UcCurChar & 0XF8) == 0XF0) //0XF8 == 11111000  0XF0 == 11110000
				{
					//11110XXX  4个1 1个0  加紧跟的3个10开头的字节	共4个字节	17~21有效bit位
					nNeedContinuousByteCount = 3;
				}
				else if ((UcCurChar & 0XF0) == 0XE0) //0XF0 == 11110000  0XE0 == 11100000
				{
					//1110XXXX  3个1 1个0  加紧跟的2个10开头的字节	共3个字节	12~16有效bit位
					nNeedContinuousByteCount = 2;
				}
				else if ((UcCurChar & 0XE0) == 0XC0) //0XE0 == 11100000  0XC0 == 11000000
				{
					//110XXXXX  2个1 1个0  加紧跟的1个10开头的字节	共2个字节   8~11有效bit位
					nNeedContinuousByteCount = 1;
				}
				else
				{
					return false;
				}
			}
		}
		else  //必需接收10xxxxxx 的跟进字节流
		{
			//多字节符的非首字节,应为 10xxxxxx 
			if ((UcCurChar & 0XC0) == 0X80)			//0XC0 == 11000000    0x80 == 10000000
			{
				if (--nNeedContinuousByteCount < 0)//减到为零为止	
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}

	if (nNeedContinuousByteCount != 0)//违返UTF8编码规则 
	{
		return false;
	}
	return true;
}
#if _LINUX
int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	//int rc;
	char **pin = (char**)&inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}
#endif
std::string string_To_UTF8(const std::string & str)
{
	std::string retStr;
#if _LINUX
	char outbuf[1024] = { 0 };
	int outlen = 1024;
	code_convert("GB18030", "utf-8", str.c_str(), str.length(), outbuf, outlen);
	retStr = outbuf;

#else
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	retStr = pBuf;;

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

#endif
	return retStr;
}
std::string UTF8_To_string(const std::string & str)
{
	std::string retStr;
#if _LINUX
	char outbuf[1024] = { 0 };
	int outlen = 1024;
	code_convert("utf-8", "GB18030", str.c_str(), str.length(), outbuf, outlen);
	retStr = outbuf;
#else
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

#endif
	return retStr;
}



