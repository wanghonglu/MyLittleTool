#pragma once
/*
	八皇后问题
	n 皇后问题研究的是如何将 n 个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击。
	上图为 8 皇后问题的一种解法。

	给定一个整数 n，返回所有不同的 n 皇后问题的解决方案。

	每一种解法包含一个明确的 n 皇后问题的棋子放置方案，该方案中 'Q' 和 '.' 分别代表了皇后和空位。
	输入: 4
	输出: [
	[".Q..",  // 解法 1
	"...Q",
	"Q...",
	"..Q."],

	["..Q.",  // 解法 2
	"Q...",
	"...Q",
	".Q.."]
	]
	解释: 4 皇后问题存在两个不同的解法。

*/
#include<string>
#include<vector>
using namespace std;
class NQueue {
public:
	void addOneResult()
	{
		std::vector<std::string> result;
		for (int i = 0; i < m_size; i++)
		{
			std::string temp(m_size,'.');
			for (int j = 0; j < m_size; ++j)
			{
				if (m_base[i][j])
				{
					temp[j] = 'Q';//每一行就一个皇后
					break;
				}
					
			}
			result.emplace_back(std::move(temp));
		}
		m_result.emplace_back(std::move(result));
	}
	bool isOk(int row, int col)//判断当前位置是否可以防止皇后
	{
		//判断 当前列有没有皇后，有就不合适
		for (int i = 0; i < m_size; ++i)
		{
			if (m_base[i][col])
				return false;
		}
		//因为是从上往下放 所以只判断左上 右上有没有皇后就行，左下右下是没有的
		//左上
		for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
		{
			if (m_base[i][j])
				return false;
		}
		//右上
		for (int i = row - 1, j = col + 1; i >= 0 && j < m_size; i--, j++)
			if (m_base[i][j])
				return false;
		return true;
	}
	void SolveNQueens(int row )
	{
		if (row == m_size)
		{
			addOneResult();
			return;
		}
		for (int i = 0; i < m_size; i++)
		{
			if( !isOk(row,i) )
				continue;
			//找到一个可以防止皇后的位置，因为一行只能放一个，所以直接找下一行
			m_base[row][i] = 1;
			//计算下一行
			SolveNQueens(row + 1);
			//回溯 重置这个点，再尝试下一列
			m_base[row][i] = 0;
		}
	}
	vector<vector<string>> solveNQueens(int n) {
		/*回溯算法*/
		m_size = n;
		for (int i = 0; i < n; i++)
		{
			m_base.push_back(std::vector<int>(n, 0));
		}
		SolveNQueens(0);
		return m_result;
	}
	std::vector<std::vector<int>>		  m_base;//棋盘
	std::vector<std::vector<std::string>> m_result;
	int									  m_size;
};