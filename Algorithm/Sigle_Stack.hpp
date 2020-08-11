#ifndef __SIGLE_STACK_HPP_
#define __SIGLE_STACK_HPP_
#include "Algorithm_test.hpp"
/*
 * 给出一个矩形统计图，它的每个矩形的宽度都为1，高度是题目所给。要你求出这个矩形图中最大面积的长方形。
 * 比如矩阵
 * 矩形统计图的数据为 [4, 3, 2, 5, 6]
 * 则最大矩形面积就是 5*5=25
 * 可以用单调栈来解决 
 * 思路是: 当前元素左右两边扩展 遇到比自己大的 就继续扩展 比自己小的就截止即元素所在最大面积由最小的决定
 * 构造单调递减栈( 即栈顶大 而栈底小 )
 * 如果当前元素大于等于栈顶入栈   则入栈 否则栈顶出栈,此时需要计算栈顶所在最大矩形面积 
 * 计算方法为 pop出来该计算元素,新的栈顶 即该计算元素的左边界 计算元素的右边界为遍历元素 有了两个边界 面积自然好算
 * */
class LargestRectanleArea:public AlgorithmSolution{
public:
    LargestRectanleArea()
    {
        m_algorithmName = "最大矩形问题  单调栈算法";
        m_index = AlgorithmSolution::s_index++;
    }
    void Solution()override
    {
        std::vector<int> nums={2,1,5,6,2,3};
        int result = getLargestArea(nums);
        std::cout<<" 原统计图： ";
        PrintArray(nums);
        std::cout<<"最大矩形面积为: "<<result<<std::endl;
    }
private:
    int getLargestArea( std::vector<int>nums )
    {
        int max = std::numeric_limits<int>::min();
        nums.push_back(max);
        Stack<int> st;
        for(int i=0;i<nums.size();i++ )
        {
            
            /*
                单调栈算法
                面积由最短的那一块决定
                栈内数据是从小到大排列
                当遇到比栈顶小的元素就可以计算当前栈顶的值了
                为了方便计算数组内最后一个元素我们设置为最小，方便弹出所有元素
                2 1 2
                //左右边界需要单独计算，这里一定不能计算错了
            */
            while(!st.empty() && nums[i]<nums[st.top()])
            {
                int temp = st.top();
                st.pop();
                int left =0,right=0;
                if(st.empty())
                    left =temp;
                else
                    left = temp - st.top()-1;
                right = i - temp;
                int area = (right+left)*nums[temp];
                if( area>max )
                    max = area;
            }
            st.push(i);
        }
        return max;
    }
};
/*
 * 描叙：有n个人站队，所有的人全部向右看，个子高的可以看到个子低的发型，给出每个人的身高，问所有人能看到其他人发现总和是多少。
 * 输入：4 3 7 1
 * 输出：2
 * 解释：个子为4的可以看到个子为3的发型，个子为7可以看到个子为1的身高，所以1+1=2
 * 因为是向右看 所以可以构造一个单调递增栈( 单调递增指的是 出栈的时候 元素是单调递增的 , 即栈顶小 而栈顶大 )
 * 然后判断 如果元素小于栈顶 则入栈
 * 否则弹出栈顶 注意这里栈顶元素能看到的人是" 当前元素的下标-栈顶元素的下标-1 "
 * 如果他俩相邻 则栈顶元素谁都看不到
 * 最后还需要把栈里的所有元素弹出来再计算  此时栈里元素能看到的人数是  最后一个人下标-当前人下标-1
 * */
class InorderLineUp:public AlgorithmSolution{
public:
    InorderLineUp()
    {
        m_algorithmName = "排队向右看问题  单调栈算法";
        m_index = AlgorithmSolution::s_index++;
    }
    void Solution()override
    {
        std::vector<int> v={1,2,1,8,1,6,3,7,3};
        int result = lookright(v);
        std::cout<<"原数组: ";
        PrintArray(v);
        std::cout<<" 能看到的人的总数为: "<<result<<std::endl;      
    }
    int lookright(std::vector<int> nums )
    {
        if( nums.empty() )
            return 0;
        Stack<int> st;
        int total=0;
        //放进去一个最高的人 保证里面的人都计算出来
        nums.push_back( std::numeric_limits<int>::max() );
        for(int i=0;i<nums.size();i++ )
        {
            while(!st.empty() && nums[i]>nums[st.top()])
            {
                int temp = st.top();
                st.pop();
                int cansee = i-temp-1;
                total+=cansee;
            }
            st.push(i);
        }
        return total;
    }
};
/*
 *  下一个最高温度问题
 *  给定一个数组,代表每天的问题, 算出一个数组每一项代表下一个更高问题要等待的天数
 *  比如输入temperatures = [73, 74, 75, 71, 69, 72, 76, 73]，你的输出应该是 [1, 1, 4, 2, 1, 1, 0, 0]。
 *  因为要找的是比自己大的元素 所以这里需要构建的是递增栈
 *  每次由比栈顶大的数 则表明找到了 当前栈顶的下一个更大的温度 比栈顶小则入栈
 *
 * */
class NextHighestTemperture:public AlgorithmSolution{
public:
    NextHighestTemperture()
    {
        m_algorithmName = "观测到下一个更高温度至少需要几天  单调栈算法";
        m_index = AlgorithmSolution::s_index++;
    }
    void Solution()override
    {
        std::vector<int> v={73, 74, 75, 71, 69, 72, 76, 73};
        std::vector<int>result = getNexttemperature(v);
        std::cout<<"原数组：";
        PrintArray(v);
        std::cout<<"观测到下一个更高温度需要的各个日期为: ";
        PrintArray(result);
    }
    std::vector<int> getNexttemperature(std::vector<int>& T)
    {
        if( T.empty() )
            return std::vector<int>();
        std::vector<int> result(T.size(),0 );
        Stack<int> st;
        for(int i=0;i<T.size();++i )
        {
            while(!st.empty() && T[i]>T[st.top()] )
            {
                result[st.top()] = i-st.top();
                st.pop();
            }
            st.push(i);
        }
        return result;
    }
};
/*
 *  接雨水问题,跟矩阵类似,给定宽度为1 不同长度的柱子 求最大能接的雨水的量
 *  比如输入 [1,0,0,2] 那么能接的雨水就是2个单位
 *  输入[0,1,0,2,1,0,1,3,2,1,2,1] 则能接雨水就是6个单位 可画图自己分析
 *  这个跟矩形面积类似  但又不同
 *  当前元素应该找左边 比它小的最大的元素
 *  故应该建立一个递减栈 栈顶大而栈底小
 *  
 *
 * */
class GetRainQuest:public AlgorithmSolution{
public:
    GetRainQuest()
    {
        m_algorithmName = "接雨水问题  单调栈算法";
        m_index = AlgorithmSolution::s_index++;
    }
    void Solution()override
    {
        std::vector<int>v={4,2,0,3,2,5};
        int result = getRain(v);
        std::cout<<"原数组: ";
        PrintArray(v);
        std::cout<<"总共能接雨水： "<<result<<std::endl;
    }
    int getRain(std::vector<int>& height)
    {
        if( height.empty() )
            return 0;
        int total;
        Stack<int> st;
        for(int i=0;i<height.size();++i)
        {
            while( !st.empty()&& height[i]>height[st.top()])
            {
                int temp = st.top();
                st.pop();
                if( st.empty() )
                    break;//左边兜不住
                //这里的长度是 左右两边推算得来的，两边算了距离后需要再减1
                int len =(i - temp)+(temp-st.top())-1;;
                int h = std::min<int>( height[i] - height[temp], height[st.top()] - height[temp]);
                int area = len*h;
                total+=area;
            }
            st.push(i);
        }
        return total;

    }
};

#endif
