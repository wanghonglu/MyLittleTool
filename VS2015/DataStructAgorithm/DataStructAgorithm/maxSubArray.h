#pragma once
/*	
������������⣺
	����һ���������� nums ���ҵ�һ���������͵����������飨���������ٰ���һ��Ԫ�أ������������͡�
	[-2,1,-3,4,-1,2,1,-5,4]
	���: 6
	����: ���������� [4,-1,2,1] �ĺ����Ϊ 6��

	���鲢��˼·һ����������ֳ������� [left,mid] [mid+1,right] �Ϳ�Խ�м�Ĳ���
	[left,mid]�������left right˼·��һ���� ���Եݹ�ȥ��
	�ұ�����һ��
	�ؼ������ ��Խ�м�����飬��Ϊ��Խ�м�����飬���Ա�Ȼ����mid Ȼ���mid��ʼ�����ҳ���������˵�
	�����ҳ��������Ҷ˵㣬���Ҷϵ�����������䣬����������ߵĺͣ�Ȼ���������������бȽϣ����ɵó�����������
	ʱ�临�Ӷ� nlogn
*/
#include<vector>
class Solution {
public:
	int maxSubarray(std::vector<int>&nums, int left, int right)
	{
		//�������֣�start mid end  �������֣�����������С�꣬�ұ�����±� Ȼ���������֮����������  
		if (left >= right)
			return nums[left];
		int mid = left + (right - left) / 2;
		int sumLeft = maxSubarray(nums, left, mid);
		int sumRight = maxSubarray(nums,mid + 1, right);
		//�ؼ��Ǽ����м�Ĳ��֣��м�Ĳ�����Ϊ��Ȼ����mid�� Ȼ���mid����� �����ҳ�����



		int sum = 0;
		int subLeftSum = INT_MIN;
		for (int i = mid;i >= left;i--)
		{
			sum += nums[i];
			if (subLeftSum<sum )
				subLeftSum =sum;
		}
		sum = 0;
		int subRightSum = INT_MIN;
		for (int i = mid + 1;i <= right;i++)
		{
			sum += nums[i];
			if (subRightSum < sum)
				subRightSum = sum;
		}
		//�м�ĵĺ� mid�������� ������Ҫ�޳�����
		int midSum = subRightSum + subLeftSum;

		//��������������ǵ�ǰ�ε�����������
		if (sumLeft >= sumRight && sumLeft >= midSum)
			return sumLeft;
		if (sumRight >= sumLeft && sumRight >= midSum)
			return sumRight;

		return midSum;
	}
	int maxSubArray(std::vector<int>& nums) {
		if (nums.empty())
			return 0;
      
		return maxSubarray(nums, 0, nums.size() - 1);
	}
};
/*	
	��̬�滮��
	����Ķ�̬�滮����ǰ�Ĳ�̫һ������ǰ�Ķ�̬�滮��Ϊf[i]=f[i-1]....
	f[i-1] �ǵ�i����i-1ʱ�Ľ⣬��Ϊ 1 2 -3 4 ��Ϊf(2)�������������1 2
	����f(3) ȴ������Ϊ��1+2+4 ��Ϊ���ǲ�����
	���ﶨ��f[i] ��i��β��������飬��������f[0] f[1] �������ҳ����������� 
	���Ը��±�Ϊ��β�����������������Ľ�
	״̬ת�Ʒ���Ϊ f[i]����iΪ�±������) = std::max( f[i]+nu[i],nu[i] )
	ʱ�临�Ӷ� O(n)
*/
class DynamicProcess {
public:
	int maxSubArray(std::vector<int>& nums) {
		if (nums.empty())
			return 0;
		std::vector<int> result(nums.size());
		int sum = nums[0];
		result[0] = nums[0];
		for (int i = 1;i < nums.size();++i)
		{
			result[i] = std::max<int>(nums[i], nums[i] + result[i - 1]);
			if (sum < result[i])
				sum = result[i];
		}
		return sum;
	}
};
//����������dp���Ը��򵥵�һ��
//���ƵĻ�������һ���˻���������飬����������в�ͬ
/*
	�˻�����ʱ����������һ�� dp[i]=std::max(num[i],num[i]+result[i-1] )
	��Ϊ��ǰֵΪ������ʱ�� �����һ���ǶԵ� ������������� -4 3 -6 f(3)=3 ����f(6)ȴ��72
	���Թ۲쵽 ��nums[1]Ϊ������ʱ�� Ҫ�˵���ǰ�����Сֵ���ܵõ����ֵ
	���Զ�̬�滮����Ҫ�洢f(i)����Сֵ
	������СֵҲͬ��  ��Ϊ������ʱ�� �ǳ˵�ǰ������ ���ܵõ���С

*/
class maxProduct {
public:
	int maxProduct(vector<int>& nums) {
		if (nums.empty())
			return 0;
		//�˻��Ļ� num[i]�������������ǰ������max(num[i],num[i]*result[i-1])
		//����Ǹ������Ͳ�һ���ˣ������Ļ�Ҫ����ǰ�����Сֵ������������Ҫ������״̬
		//һ�����һ����С
		//ͬ��ȡ������ʱ��ҲҪ����Ӧ���ж�
		std::vector<int> result(nums.size());
		std::vector<int>min(nums.size());
		min[0] = nums[0];
		result[0] = nums[0];
		int sum = nums[0];
		for (int i = 1;i < nums.size();++i)
		{
			if (nums[i] >= 0)
			{
				result[i] = std::max<int>(nums[i], nums[i] * result[i - 1]);
				min[i] = std::min<int>(nums[i], nums[i] * min[i - 1]);
			}
			else
			{
				result[i] = std::max<int>(nums[i], nums[i] * min[i - 1]);
				min[i] = std::min<int>(nums[i], result[i - 1] * nums[i]);
			}
			if (result[i] > sum)
				sum = result[i];
		}
		return sum;
	}
};