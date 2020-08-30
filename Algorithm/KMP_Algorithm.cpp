
#include "BaseAlgorithm.hpp"
/*
    KMP算法看了好久 总算看了个差不多，先说原理
    加入目标串是 "abcabmfaggef", 模式串是 "abcabef"
    如果是暴力方案的话 做法是对目标串做遍历，从a->f 针对每个字符作为开头一个一个跟模式串比较 
    比如比较到m的话 m->a比较  依次类推
    这样的时间复杂度是O(m*n)  ---》不能忍

    但是我们看 从a开始比较  到m的时候 m!=e
    暴力方式 又回溯到了b
    但是 我们发现  模式串e的前面 也就是 abcab中 前两个和后两个一模一样，同时目标串中也还有这五个字符
    我们可以吧 目标串中的 后面的ab当做模式串中前面的ab 然后用目标串的 e和模式串的c去做比较
    这样做  目标串是不回回溯的 因为一旦后面的不匹配 我们就用前面字符串中  源字符串的最大后缀来匹配模式串中的最大前缀
    继续接着往下找

    所以我们需要一个数组记录一下  当p[i]出现不匹配时  p[i-1]中最大前缀等于后缀的值是多少，然后用这个值 p[p[i-1]] 
    跟s[i]继续往下比较
    这就是next数组

    next[i] 表示 P[0] ~ P[i] 这一个子串，
    使得 前k个字符恰等于后k个字符 的最大的k. 特别地，
    k不能取i+1（因为这个子串一共才 i+1 个字符，自己肯定与自己相等，就没有意义了）。

    KMP算法的核心是构建next数组
    next数组只跟模式串有关，跟源字符串没有关系，时间复杂度是O(n)
    
    next数组可以通过动态规划求得。如果要求next[i] 假设我们已知 next[i-1]的情况下
    next[i-1]表示 在模式串0-i-1中 最大的前 next[i-1]=k 等于最大的后缀

    这样只需要比较第 p[k]和第p[i]是否相等即可，如果他们相等
    很简单，next[i]=next[i-1]+1//等于最大k匹配扩长了一位
    不相等的时候 稍微麻烦一点，但能想通！

    举个例子
    a   b   c   a   b   d   d   d   a   b   c   a   b   c
    0   0   0   1   2   0   0   0   1   2   3   4   5
    假如我们要计算最后的那个c 则已知前面的b的next[b]=5 这个5我们设置为now 即

   !!!! 这里切记 next数组不等于0的时候一定是包含第一个和第i个！！！！！
    假设此时c的下标为i
    则前i-1个串中 前五个是等于后五个的
    同样的  前五个串中 最大前缀也是等于后五个串中的最大后缀的！！！
    
    c前面的 ab 和 最前面的ab是相等的
    即此时 这个ab其实是第五个字符b的next数组的值 即next[now-1] 这样 只需要跟 p[2]和c比较即可
    相等就+1
    不相等再次缩短 直到缩短到now为0或者p[now]=p[i]为止
    比较复杂 等多想想
*/
#include<string>
#include<vector>
class KMP_Algorithm:public AlgorithmSolution{
    public:
    KMP_Algorithm()
    {
        m_algorithmName ="字符串匹配算法 KMP算法";
    }
    //获取next数组
    std::vector<int> getNext(const std::string&pattern)
    {
        //首先给填充个默认值都是0
        std::vector<int> next(pattern.size(),0);
        int i=1;//从1开始 因为0号为止的next数组一定是0
        int k=0;//k代表 i-1字符串里 满足最大前k个前缀等于最大后k个后缀
    
        for(;i<pattern.size(); )
        {
            //如果第i个字符跟 i-1的next数组下标对应的值相等 则 next[i]=k+1;
            if(pattern[i] == pattern[k] )
            {
                next[i] = k+1;
                k++;
                i++;//往后继续计算 直到计算完
            }
            //如果不相等了 就稍微麻烦些
            else if( k>0 )
            {
            //即 i前面有k个前缀 等于k个后缀冰且当前i跟k位置元素不相等，此时缩小k的范围
            //缩小为前k个元素中的 最大前k匹配最大后k  即next[k-1]
                k = next[k-1];
            }
            else
            {
                //即不相等且k已经倒退到0，则此时i的next数组为0
                next[i]=0;
                i++;//继续计算下一个元素
            }
            
        }
        return next;
    }
    void Solution()override
    {
        std::string  src="mississippi";
        std::string  pattern="issip";
      
        int pos = strStr(src,pattern);
        std::cout<<"KMP算法查找第一次匹配的位置 "<<std::endl;
        std::cout<<" 主串: "<<src<<std::endl;
        std::cout<<" 模式串: "<<pattern<<std::endl;
        std::cout<<"第一次出现模式串的位置："<<pos<<std::endl;
    }
    int strStr(const std::string& haystack, const std::string& needle) 
    {
        //获取next数组代表每个i下最大的k 使得前k个元素跟后k个元素相等 必然包含第0个和第i个
        if( needle.empty() || haystack == needle )
            return 0;
        std::vector<int> next = getNext(needle);
        PrintArray(next);
        int pos = -1;
        int j=0;//模式串下标
        for( int i=0;i<haystack.size(); )
        {
            if( haystack[i] == needle[j] )
            {
                j++;
                i++;
            }
            else
            {
                //不相等则按照next数组决定 j的新位置去比较
                if( j>0 )
                    j = next[j-1];
                else 
                    i++;
            }
            //当j等于size-1的时候即 找到匹配位置了 直接退出即可
            if( needle.size()==j )
            {
                pos = i-needle.size();
                break;
            }
            
        }
        return pos;
    }

};
//字符串匹配算法 KMP算法
RegistAlgorithm(KMP_Algorithm)
