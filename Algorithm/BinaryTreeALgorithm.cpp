
/*
    二叉树最大路径和
    给定一个非空二叉树，返回其最大路径和。

    本题中，路径被定义为一条从树中任意节点出发，达到任意节点的序列。该路径至少包含一个节点，且不一定经过根节点。
    输入: [1,2,3]

       1
      / \
     2   3

    输出: 6
    输入: [-10,9,20,null,null,15,7]

    -10
     / \
    9  20
     /  \
     15   7

    输出: 42

    这个应该没啥别的思路，只能递归的一点一点的算，
    算出左右子树的最大和，然后判断给出结果
*/
#include "BaseAlgorithm.hpp"
#include<map>
class BTMaxPathSum:public AlgorithmSolution
{
    public:
    int   m_maxpathsum=INT_MIN;
    BTMaxPathSum()
    {
        m_algorithmName = "二叉树最大路径和  后续遍历 ";
    }
    void Solution()override
    {
        //BinaryTree<int> bt={-10,9,20,INT_MIN,INT_MIN,15,7};
        BinaryTree<int> bt={-2,INT_MIN,-3};
        std::cout<<"原来的树"<<std::endl;
        bt.levelOrder();
        maxPathSum(bt.GetRoot() );
        std::cout<<"最大路径和为: "<<m_maxpathsum<<std::endl;
    }
    template<typename T>
    int maxPathSum(TreeNode<T>* node) 
    {
        if( node == nullptr )
            return 0;
        int left_max = std::max<int>(0,OnsideMaxPath(node->left_));
        int right_max = std::max<int>(0,OnsideMaxPath(node->right_));

        m_maxpathsum = std::max<int>(m_maxpathsum, left_max+right_max+node->val);
        maxPathSum(node->left_);
        maxPathSum(node->right_);
        return m_maxpathsum;
    }
    template<typename T>
    int OnsideMaxPath(TreeNode<T>* node )
    {
        if(node == nullptr)
            return 0;
        int left = std::max<int>(0,OnsideMaxPath(node->left_));
        int right = std::max<int>(0,OnsideMaxPath(node->right_));

        return std::max(left,right)+node->val;
    }
};
RegistAlgorithm(BTMaxPathSum)