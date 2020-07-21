#pragma once
/*
数组nums包含从0到n的所有整数，但其中缺了一个。请编写代码找出那个缺失的整数。你有办法在O(n)时间内完成吗？
最小的是0 最大的是 nums.size()
按照存在的情况计算出和 等差数列求和公式，然后减去实际的和就是确实的那个数
*/
#include<vector>
class MissingNumber {
public :
	int missingNumber(std::vector<int>& nums) {
		if (nums.empty())
			return 0;
		int sum = nums[0];
		for (int i = 1; i < nums.size(); i++)
		{
			sum += nums[i];
		}
		int realSum = (0 + nums.size())* (nums.size() + 1) / 2;
		return realSum - sum;
	}
};
/*
	类似的第二个题 letcode 41
	给你一个未排序的整数数组，请你找出其中没有出现的最小的正整数。
	示例 1:

	输入: [1,2,0]
	输出: 3
	示例 2:

	输入: [3,4,-1,1]
	输出: 2
	示例 3:

	输入: [7,8,9,11,12]
	输出: 1

	这个题比上面那个难的多
*/