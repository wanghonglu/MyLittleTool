/*
 * 算法描述:
 *          从一个给定的数组中找出不重复的两个数,其他数字都重复了两次
 *          比如给出的数组 a[]={1,2,1,2,3,4}
 *          输出3 4
 *
 *          解题原理类似交换两个值
 *          a = a^b;
 *          b = a^b;
 *          a = a^b;
 *          解题思路,第一步将整个数组全部异或,这样便能得到数组里面 不重复的两个数的异或结果
 *          以为重复的数异或为0 0与任何数异或得到的结果都是本身
 *          得到之后,找到这个数里面第一个比特位为1的数 那么这两个数 第N为一个为0 一个为1
 *          将这组数分成两组 N位为0的一组和 N位为1的一组,然后分别把两组元素异或 即得到这两个数
 * */
#include<stdio.h>
#include<vector>
#include"BaseAlgorithm.hpp"
class FindUnique_number:public AlgorithmSolution{
    public:
    FindUnique_number()
    {
        m_algorithmName = "查找唯一不重复的数据 异或交换两数";
    }
    void Solution()override
    {
        AlgorithmSolution::Solution();
        std::vector<int> nums={1,7,8,1024,9,188823,1024,1,1987,188823,8,5,5,9};
        int base=0;
        //这个base是那个两个不重复的数 异或后的结果 因为重复的数异或后为0 0异或任何数都是其本身

        //找出这个数的第一个比特位为1的数,假定其为N  则数组分为两部分 第N位位1 和不为1的
        //然后用number 分别去异或这两部分 就可以分别得到这两个数, 原理类似 
        for( int a:nums )
        {
            base^=a;
        }
        base = find_first_bit_1(base);
        int first=0,second=0;
        for( int a:nums )
        {
            if( a & base)
                first ^=a;
            else
                second ^=a;
        }
        std::cout<<"原数组: ";
        PrintArray(nums);
        std::cout<<std::endl;
        std::cout<<"唯一不重复的数据是 "<<first<<"  "<<second<<std::endl;
        return;
    }
    int find_first_bit_1( int val )
    {
        int a =0;
        while(val)
        {
            a = val;
            val &=(val-1);
        }
        return a;
    }
};
RegistAlgorithm(FindUnique_number)