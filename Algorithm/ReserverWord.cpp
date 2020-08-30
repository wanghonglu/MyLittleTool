#include "BaseAlgorithm.hpp"
/*
给定一个字符串，你需要反转字符串中每个单词的字符顺序，同时仍保留空格和单词的初始顺序。
输入："Let's take LeetCode contest"
输出："s'teL ekat edoCteeL tsetnoc"
*/
class ReverseWork:public AlgorithmSolution{
    public:
    ReverseWork()
    {
        m_algorithmName="反转单词";
    }
    void Solution()override
    {
        std::string str="Let's take LeetCode contest";
        std::string ret = reverseWords(str);
        std::cout<<"原字符串: "<<str<<" 反转后: "<<ret<<std::endl;
    }
    std::string reverseWords(const std::string s) {
        std::string ret;
        auto pos = s.find_first_of(" ", 0);
        size_t beg =0;
        std::string temp;
        while(pos !=std::string::npos )
        {
            temp=s.substr(beg,pos-beg);
            std::reverse(temp.begin(),temp.end() );
            ret+=temp;
            beg = s.find_first_not_of(" ",pos+1);
            pos = s.find_first_of(" ", beg);
            ret+=" ";     
        }
        if(beg<pos)
        temp = s.substr(beg,pos-beg);
        std::reverse(temp.begin(),temp.end() );
        ret+=temp;
        return ret;
    }
};
RegistAlgorithm(ReverseWork)