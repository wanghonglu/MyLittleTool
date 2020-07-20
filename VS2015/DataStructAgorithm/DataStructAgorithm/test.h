#include<vector>
#include<functional>
#include<algorithm>
void insert_sort(std::vector<int>& nums)
{
	if (nums.empty())
		return;
	for (int i = 1;i < nums.size();++i)
	{
		if (nums[i] < nums[i - 1])
		{
			int temp = nums[i];
			int j = i - 1;
			for (;j >= 0;j--)
			{
				if (nums[j] <= temp)
					break;
				nums[j + 1] = nums[j];
			}
			nums[j+1] = temp;
		}
	}
}
void _merge_sort_recursive(std::vector<int>&nums, std::vector<int>&result, int left, int right)
{
	if (right - left <= 1)
	{
		if (1 == right - left && nums[left] > nums[right])
			std::swap(nums[left], nums[right]);
		return;
	}
	int mid = left + (right - left) / 2;
	//����
	_merge_sort_recursive(nums, result, left, mid);
	_merge_sort_recursive(nums, result, mid + 1, right);
	//�鲢
	int start = left;
	int start1 = left, start2 = mid + 1;
	while (start1 <= mid && start2 <= right)
	{
		result[start++] = nums[start1] < nums[start2] ? nums[start1++] : nums[start2++];
	}
	//�鲢ʣ������
	while (start1 <= mid)
		result[start++] = nums[start1++];
	while (start2 <= right)
		result[start++] = nums[start2++];
	//������õ����ݿ�����ԭ����
	for (int i = left;i <= right;++i)
		nums[i] = result[i];
}
void merge_sort_recursive(std::vector<int>&nums)
{
	std::vector<int> result(nums.size());
	_merge_sort_recursive(nums, result, 0, nums.size() - 1);
	nums.swap(result);
	return;
}
//������Ĺ鲢
void merge_sort(std::vector<int>& nums)
{
	//�ݹ��Ǵ��ϵ��� �����Ǵ��µ���
	std::vector<int>result(nums.size());
	int n = nums.size();
	for (int step = 1;step < n ;step *= 2)
	{
		for (int i = 0;i < n;i += 2*step)//��1��ʼ�з� ��sep=n/2��ʱ�� �ұ���ָ�ı߽�
		{
			int start = i;
			int left = i;
			int right = std::min<int>(i + 2*step - 1, nums.size() - 1);
			int mid = left + (right - left) / 2;
			int start1 = left, start2 = mid + 1;

			while (start1 <= mid && start2 <= right)
			{
				result[start++] = nums[start1] < nums[start2] ? nums[start1++] : nums[start2++];
			}
			while (start1 <= mid)
				result[start++] = nums[start1++];
			while (start2 <= right)
				result[start++] = nums[start2++];
		}
		result.swap(nums);//���ﱣ֤�� nums���������� �����ս���������
	}
}
void _quick_sort(std::vector<int>&nums, int start, int end)
{
	if (end - start <= 1)
	{
		if (1 == end - start && nums[start] > nums[end])
			std::swap(nums[start], nums[end]);
		return;
	}
	int pivot = nums[end];
	int start1 = start, start2 = end;
	while (start1 < start2)
	{
		while (start1 < start2 && nums[start1] <= pivot)
			start1++;
		if (start1 < start2)
			nums[start2--] = nums[start1];
		while (start1 < start2 && nums[start2] >= pivot)
			start2--;
		if (start1 < start2)
			nums[start1++] = nums[start2];
	}
	nums[start1] = pivot;
	_quick_sort(nums, start, start1 - 1);
	_quick_sort(nums, start1 + 1, end);

}
void quick_sort(std::vector<int>&nums)
{
	return _quick_sort(nums, 0, nums.size() - 1);
};
void shiftdown(std::vector<int>&nums, int pos,int len)
{	
	int temp = nums[pos];
	int parent = pos;
	int son = parent * 2 + 1;
	while (son < len)
	{
		if (son + 1 < nums.size() && nums[son + 1] > nums[son])
			son += 1;
		if (temp >= nums[son])
			break;
		nums[parent] = nums[son];
		parent = son;
		son = 2 * parent + 1;
	}
	nums[parent] = temp;

}
void heap_sort(std::vector<int>&nums)
{
	//�����һ�����ڵ㿪ʼ���µ��� ԭ�ؽ��� 
	//���һ���ӽڵ�Ϊ nums.size()-1 ���ĸ��ڵ�Ϊ(nums.size()-1-1)/2 ��nums.size()/2-1
	for (int i = nums.size() / 2 - 1;i >= 0;i--)
	{
		shiftdown(nums, i,nums.size()-1);
	}
	//���� ���δӴ󶥶�����ȡ��ֵ�������
	for (int i = nums.size() - 1; i >= 0;i--)
	{
		std::swap(nums[i], nums[0]);
		shiftdown(nums, 0, i);
	}

}
void shell_sort(std::vector<int>& nums)
{
	for (int sep = nums.size() / 2;sep >= 1;sep /= 2)
	{
		for (int i = sep;i < nums.size();i++)
		{
			if (nums[i] >= nums[i - sep])
				continue;
			int temp = nums[i];
			int j = i;
			for (;j >= sep;j -= sep)
			{
				if (nums[j-sep] > temp)
					nums[j] = nums[j-sep];
				else
					break;
			}
			nums[j] = temp;
		}
	}
}