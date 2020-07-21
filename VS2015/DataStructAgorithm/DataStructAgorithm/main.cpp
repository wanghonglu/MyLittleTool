#include "test.h"
#include<iostream>
#include<algorithm>
#include<random>
#include<ctime>
#include "maxSubArray.h"
#include "getmissnumber.h"
#include "Nqueue.h"
#define Size 100000
void getrandom(std::vector<int>& v)
{
	static bool init = false;
	if (!init)
	{
		srand(time(NULL));
		init = true;
	}
	for (int i = 0;i < Size;i++)
		v.push_back(rand());
}
using namespace std;
#define PrintA(a) std::for_each(a.begin(),a.end(),[](decltype(a[0]) v){cout<<v<<" ";});cout<<endl;
int main()
{
	std::vector<int> v{ 90,1,0,17,333,1000,-10 };
	std::vector<int>v2(v);

	//insert_sort(v);
	PrintA(v);

	v = v2;
	//merge_sort_recursive(v);
	//PrintA(v);

	//merge_sort(v);
	//quick_sort(v);
	//heap_sort(v);
	//shell_sort(v);
	//PrintA(v);
	{
		std::vector<int> v = { -1,-2,-3,0 };
		cout << Solution().maxSubArray(v)<<endl;
		cout << DynamicProcess().maxSubArray(v) << endl;
	}
	{
		std::vector<int> v = { 0,1 };
		cout<<MissingNumber().missingNumber(v)<<endl;
	}
	{
		std::vector<std::vector<std::string>>ret;
		ret = NQueue().solveNQueens(4);
		cout << ret.size() << endl;
		for (int i = 0; i < ret.size(); i++)
		{
			cout << "[ "<<endl;
			for (int j = 0; j < ret[i].size(); j++)
				cout << "\t"<<ret[i][j] <<"  " <<endl;
			cout << "]"<<endl;
		}
	}
	cout << "test end" << endl;
	cin.get();

}