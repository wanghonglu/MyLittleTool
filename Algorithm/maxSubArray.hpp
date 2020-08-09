#ifndef __MAXSUBARRAY__H
#define __MAXSUBARRAY__H
/*	
最大子数组问题：
	给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
	[-2,1,-3,4,-1,2,1,-5,4]
	输出: 6
	解释: 连续子数组 [4,-1,2,1] 的和最大，为 6。

	跟归并的思路一样，将数组分成三部分 [left,mid] [mid+1,right] 和跨越中间的部分
	[left,mid]这个跟求left right思路是一样的 可以递归去解
	右边数组一样
	关键在这个 跨越中间的数组，因为跨越中间的数组，所以必然包含mid 然后从mid开始往左找出和最大的左端点
	往右找出和最大的右端点，左右断电就是它的区间，和是左边两边的和，然后对这三个数组进行比较，即可得出最大的子数组
	时间复杂度 nlogn

	动态规划：
	这里的动态规划跟以前的不太一样，以前的动态规划认为f[i]=f[i-1]....
	f[i-1] 是当i等于i-1时的解，因为 1 2 -3 4 因为f(2)的最大子数组是1 2
	但是f(3) 却不能认为是1+2+4 因为他们不连续
	这里定义f[i] 以i结尾的最长子数组，即将来对f[0] f[1] 做排序找出他们中最大的 
	即以改下表为结尾的数组才是整个数组的解
	状态转移方程为 f[i]（以i为下表的数组) = std::max( f[i]+nu[i],nu[i] )
	时间复杂度 O(n)


	乘积最大的时候不能向和最大一样 dp[i]=std::max(num[i],num[i]+result[i-1] )
	因为当前值为负数的时候 结果不一定是对的 比如数组如果是 -4 3 -6 f(3)=3 但是f(6)却是72
	可以观察到 当nums[1]为负数的时候 要乘的是前面的最小值才能得到最大值
	所以动态规划还需要存储f(i)的最小值
	计算最小值也同样  当为负数的时候 是乘的前面的最大 才能得到最小
*/
#include<vector>
#include "Algorithm_test.hpp"
class MaxSumArray:public AgorithmSolution{
	public:
	MaxSumArray()
	{
		m_algorithmName = "最大和子数组";
	}
	void Solution()override
	{
		std::vector<int> v={1,-1,10,-8,5,7,-3,4,-120};
		int result1 = DynamicProcess(v);
		int result2 = GetMaxSum(v,0,v.size()-1);
		std::cout<<"原数组: "<<std::endl;
		PrintArray(v);
		std::cout<<"通过动态规划求出的最大子数组的和为: "<<result1<<std::endl;
		std::cout<<"通过分治算法求出的最大子数组的和为："<<result2<<std::endl;
	}
	//动态规划解法，时间复杂度O(n)
	int  DynamicProcess(const std::vector<int>& nums )
	{
		if(nums.empty())
			return 0;
		int maxsum = nums[0],beforelesum=nums[0];//beforelesum 以前面i结尾的元素最大的数组
		for( int i=1;i<nums.size();i++ )
		{
			beforelesum = std::max<int>(nums[i], nums[i]+beforelesum );
			if( beforelesum>maxsum )
				maxsum = beforelesum;
		}
		return maxsum;
	}
	private:
	//分治法实现 时间复杂度O(logn)
	int GetMaxSum(const std::vector<int>& nums,int begin,int end )
	{
		//分治的思想是，最大子数组，要么存在在左半个数组中，要么存在在右半个数组中，要么跨过mid元素的数组中
		//左半个数组和右半个数组跟求原始数组的思路一样
		//中间部分得单独求一下
		//函数最后返回三个值中最大的即可
		if( begin>=end )
		{
			return nums[begin];
		}
		int mid = (begin+end)/2;
		int sumleft = GetMaxSum(nums,begin,mid);
		int sumright = GetMaxSum(nums,mid+1,end);
		//计算中间，必然包括mid，所以可以从mid起始计算左边和右边的最大
		int leftsum=std::numeric_limits<int>::min();
		int sum=0;
		for( int i=mid;i>=begin;i-- )
		{
			sum+=nums[i];
			if(sum>leftsum)
				leftsum = sum;
		}
		sum = 0;
		int rightsum = std::numeric_limits<int>::min();
		for(int i=mid+1;i<=end;i++ )
		{
			sum +=nums[i];
			if(sum>rightsum )
				rightsum = sum;
		}
		int crossum = leftsum+rightsum;
		if( sumleft>=crossum && sumleft>=sumright )
			return sumleft;
		if( sumright>=crossum && sumright>=sumleft )
			return sumright;
		return crossum;
	}


};
#endif
class MaxProductArray:public AgorithmSolution{
	public:
	MaxProductArray()
	{
		m_algorithmName = "最大乘积子数组 动态规划";
	}
	void Solution()override
	{
		std::vector<int> v={-1,2,-3,-4,9,10,0,-100,-4};
		int result = getMaxProductSubArray(v);
		std::cout<<"原数组: ";
		PrintArray(v);
		std::cout<<std::endl<<" 最大乘积子数组的值为 "<<result<<std::endl;
	}
	private:
	int getMaxProductSubArray(const std::vector<int>nums )
	{
		//乘积得分正负处理，正数乘以以前一个结尾的最大，负数乘以以前一个结尾的最小，必须算出最小来
		//算最小又是正数乘以最小，负数乘以最大
		if(nums.empty())
			return 0;
		int max=nums[0],min=nums[0];
		int maxproduct = nums[0];
		for(int i=1;i<nums.size();i++ )
		{
			if( nums[i]>=0 )
			{
				max = std::max<int>(nums[i],nums[i]*max );
				min = std::min<int>(nums[i],nums[i]*min );
			}
			else
			{
				int temp = max;
				max = std::max<int>(nums[i],nums[i]*min );
				min = std::min<int>(nums[i],nums[i]*temp);//这里上一步已经改了max 这个地方用的max其实是上一步的值
			}
			if(max>maxproduct)
				maxproduct = max;
		}
		return maxproduct;
	}

};
// class Solution {
// public:
// 	int maxSubarray(std::vector<int>&nums, int left, int right)
// 	{
// 		//将数组拆分，start mid end  两个部分，算出左边最大的小标，右边最大下标 然后算出两个之间的最大数组  
// 		if (left >= right)
// 			return nums[left];
// 		int mid = left + (right - left) / 2;
// 		int sumLeft = maxSubarray(nums, left, mid);
// 		int sumRight = maxSubarray(nums,mid + 1, right);
// 		//关键是计算中间的部分，中间的部分认为必然包含mid点 然后从mid点从左 从右找出最大和



// 		int sum = 0;
// 		int subLeftSum = INT_MIN;
// 		for (int i = mid;i >= left;i--)
// 		{
// 			sum += nums[i];
// 			if (subLeftSum<sum )
// 				subLeftSum =sum;
// 		}
// 		sum = 0;
// 		int subRightSum = INT_MIN;
// 		for (int i = mid + 1;i <= right;i++)
// 		{
// 			sum += nums[i];
// 			if (subRightSum < sum)
// 				subRightSum = sum;
// 		}
// 		//中间的的和 mid算了两次 这里需要剔除依次
// 		int midSum = subRightSum + subLeftSum;

// 		//返回三个中最大即是当前段的最大子数组和
// 		if (sumLeft >= sumRight && sumLeft >= midSum)
// 			return sumLeft;
// 		if (sumRight >= sumLeft && sumRight >= midSum)
// 			return sumRight;

// 		return midSum;
// 	}
// 	int maxSubArray(std::vector<int>& nums) {
// 		if (nums.empty())
// 			return 0;
      
// 		return maxSubarray(nums, 0, nums.size() - 1);
// 	}
// };

// class DynamicProcess {
// public:
// 	int maxSubArray(std::vector<int>& nums) {
// 		if (nums.empty())
// 			return 0;
// 		std::vector<int> result(nums.size());
// 		int sum = nums[0];
// 		result[0] = nums[0];
// 		for (int i = 1;i < nums.size();++i)
// 		{
// 			result[i] = std::max<int>(nums[i], nums[i] + result[i - 1]);
// 			if (sum < result[i])
// 				sum = result[i];
// 		}
// 		return sum;
// 	}
// };
// //这类问题用dp明显更简单的一点
// //类似的还处理了一个乘积最大子数组，跟上面的稍有不同
// /*


// */
// class maxProduct {
// public:
// 	int maxProduct(vector<int>& nums) {
// 		if (nums.empty())
// 			return 0;
// 		//乘积的话 num[i]如果是整数，则当前最大就是max(num[i],num[i]*result[i-1])
// 		//如果是个负数就不一样了，负数的话要乘以前面的最小值，所以这里需要存两个状态
// 		//一个最大一个最小
// 		//同样取负数的时候也要做相应的判断
// 		std::vector<int> result(nums.size());
// 		std::vector<int>min(nums.size());
// 		min[0] = nums[0];
// 		result[0] = nums[0];
// 		int sum = nums[0];
// 		for (int i = 1;i < nums.size();++i)
// 		{
// 			if (nums[i] >= 0)
// 			{
// 				result[i] = std::max<int>(nums[i], nums[i] * result[i - 1]);
// 				min[i] = std::min<int>(nums[i], nums[i] * min[i - 1]);
// 			}
// 			else
// 			{
// 				result[i] = std::max<int>(nums[i], nums[i] * min[i - 1]);
// 				min[i] = std::min<int>(nums[i], result[i - 1] * nums[i]);
// 			}
// 			if (result[i] > sum)
// 				sum = result[i];
// 		}
// 		return sum;
// 	}
// };