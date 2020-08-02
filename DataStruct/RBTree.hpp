#ifndef __RBTREE__H__
#define __RBTREE__H__
#include "BinarySearchTree.hpp"
/*
	红黑树性质:
	除了满足正常的二叉搜索树外 还额外包含以下性质
	1: 每个结点是黑色或者红色
	2: 根节点是黑色
	3: 叶子结点是黑色,叶子结点指的为nullptr的结点
	4: 不能出现连续的两个红色结点,即红色结点的孩子结点必须是黑色
	5: 从根节点开始所有路径上的黑色结点数量是一致的

	哨兵节点,根节点的父节点,叶子节点的左右孩子节点等,这里不用nullptr 而用哨兵节点来代替
	这样做本质上根nullptr节点没多大区别,但有一个好处,在find操作中,如果没找到可以返回这个哨兵节点
	总比空指针好! 可以在这个哨兵节点上设置默认值,返回默认值

	黑高:	从红黑树的任意结点x出发,不包含结点x,到达外部结点的任意路径上的黑节点的数目叫做结点x的黑高

	就查找性能而言 AVL树是稳定的logn 而红黑树则可能达到2lgn的时间复杂度,即一条路径上的红色结点和黑色结点的数目一样,
	另一条路径上全是黑色结点,这样前面的树查找时间复杂度是O(2lgn) 但是10Y数据 AVL才30多次 红黑树才60次 所以差的不是很多

	插入的时候默认待插入的几点是红色的,因为黑色的话 就破坏了红黑树第五条性质,红色结点则要满足第四条性质,需要进行旋转

	插入的调整有变色和旋转两种:
	当父节点时黑色时不调整

	当出现父节点红色的情况需要调整,不满足性质4
	一:变色=>如果叔叔结点也是红色,则改变父节点和叔叔结点的颜色为黑色,并将祖父结点改变成红色,即可
		此时祖父结点变成红色,然后再往上继续调整,如果祖父结点的父节点是根节点,则变成黑色,此时黑高增加1,这是唯一会增加黑高的情况
		比如
		黑					红
	红		红  ->		黑		黑
红					红	
	二: 叔叔结点是黑色的情况或者不存在的情况
		1:左左情况,父节点是祖父结点的左孩子,当前结点是父节点的左孩子
		做一次以祖父结点为基准右单旋转,并且交换父节点和祖父结点的颜色  
		比如
			黑				   黑
		红		黑	->		红		红
	红									黑
		2:右右情况,父节点是祖父结点的右孩子,当前结点时父节点的右孩子
		将父节点和祖父结点交换颜色,然后以祖父结点为基准左旋
		比如
		黑							黑
	黑		红			->		红		红
				红			黑
		3: 左右  父节点时祖父结点的左孩子,当前结点是父节点的右孩子
		先对父节点左旋,然后得到左左情况 变色,最祖父结点进行右旋
		比如
		黑				黑				黑
	红		黑 ->	红		黑->	红		红
		红		 红								黑
		4:右左  父节点是祖父结点的右孩子,当前结点是父节点的左孩子
		先对父节点点右旋,然后就是右右的情况,变色,对祖父结点进行左旋
		比如
		黑                 黑                      黑
	黑		红	->		黑	   红		->		红		红
		红						   红		黑
		还有就是一些特殊情况,比如插入时树为空,则变黑色直接插入
		还有就是 如果插入的结点已经存在,将颜色改为原来的颜色,然后修改value即可
		当插入一个结点的时候,父节点如果时红色,则必然是存在祖父结点的,因为根节点是黑色

		删除:
		删除比插入更加复杂:
			根AVL或者BST树类似,删除一个结点的时候,先判断有几个孩子,有一个替换 有两个找前驱或者后继,没有直接删除
			但是考虑到红黑树的性质,就要做不一样的处理了.
			1:待删除结点没有孩子
				1.1:如果待删除结点是红色的,直接删除即可,因为红色结点不影响黑高
			
*/
#include "queue.hpp"
#include <vector>
namespace datastruct {
	#define NodeIsBlack(node)  node->color_==COLOR_BLACK
	#define NodeIsRed(node)	   node->color_ == COLOR_RED
	#define SetNodeBlack(node) node->color_ = COLOR_BLACK
	#define SetNodeRed(node)   node->color_ = COLOR_RED
	template<typename Key,typename Value>
	class RBTree :public BaseBinarySearchTree<Key, Value> {
		enum NodeColor{
			COLOR_RED,
			COLOR_BLACK,
		}
		struct Node {
			Key				key_;
			Value			val_;
			Node*			left_;
			Node*			right_;
			Node*			parent_;
			NodeColor		color_;
			explicit Node(const Key& key, const Value& val, Node* senntienl) :key_(key), val_(val), left_(senntienl),
				right_(senntienl), parent_(senntienl), color_(COLOR_RED) {}
		};
	public:
		RBTree() :root_(nullptr), size_(0)
		{
			sentienl_ = new Node(Key(), Value());
			SetNodeBlack(sentienl_);//哨兵节点是黑色的
			root_ = sentienl_;
		}
		~RBTree()
		{
			destruct(root_);
			delete sentienl_;
			size_=0;
		}
		size_t  size()const override
		{
			return size_;
		}
		size_t height()const override
		{
			return height(root_);
		}
		const Value& findmax()const override
		{
			Node* node = root_;
			while( node->right_!=sentienl_)
				node = node->right_;
			return node->val_;	
		}
		const Value& findmin()const override
		{
			Node* node=root_;
			while( node->left_!=sentienl_ )
				node = node->left_;
			return node->val_;
		}
		//前驱 跟BST没有本质的区别，有左子树，则左子树最大，否则就是从当前节点往上，第一个有右孩子节点的节点
		bool getprenode(const Key&key, Value& val )const override
		{
			Node* node = root_, *firstHaveRightChild = nullptr, 
			while( node !=sentienl_ && node->key_ !=key )
			{
				if( key>node->key_ )
				{
					firstHaveRightChild = node;
					node = node->right_;
				}
				else
					node = node->left_;
			}
			if( node == sentienl_ )
				return false;
			
			if( node->left_ !=sentienl_ )
			{
				node  = node->left_;
				while( node->right_ !=sentienl_)
					node = node->right_;
				val = node->val_;
				return true;
			}
			//不存在左子树看从下网上第一个有右孩子的父节点
			if(	firstHaveRightChild!=nullptr )
				val = firstHaveRightChild->val_;
			return firstHaveRightChild!=nullptr;
		}
		//后继 跟BST也没有区别
		bool getpostnode(const Key& key, Value& val )const override
		{
			Node *node = root_, *firstHaveLeftChild = nullptr;
			while(node !=sentienl_ && node->key_ !=key )
			{
				if( key <node->key_ )
				{
					firstHaveLeftChild = node;
					node = node->left_;
				}
				else
					node = node->right_;
			}
			if( node == sentienl_ )
				return false;
			//右子树的最小
			if( node->right_!=sentienl_ )
			{
				node = node->right_;
				while( node->left_!=sentienl_ )
					node = node->left_;
				val = node->val_;
				return true;
			}
			if( firstHaveLeftChild != nullptr )
				val = firstHaveLeftChild->val_;
			return firstHaveLeftChild!=nullptr;
		}
		using DealKey = typename BaseBinarySearchTree<Key,Value>::DealKey;
		//三序递归遍历
		void preorder(const DealKey& deal )const override
		{
			_preorder(root_);
		}
		void inorder(const DealKey& deal )const override
		{
			_inorder(root_);
		}
		void postorder(const DealKey& deal )const override
		{
			_postorder(root_);
		}
		//层序遍历
		void levelorder(const DealKey& deal )const override
		{
			if( size() == 0 )
				return;
			ArrayQueue<Node*> q;
			Node *node;
			q.push(root_);
			while( !q.empty() )
			{
				node = q.front();
				q.pop();
				deal(node);
				if( node->left_!=sentienl_)
					q.push(node->left_);
				if( node->right_ != sentienl_ )
					q.puhs( node->right_);
			}
		}
		//非递归遍历的三序遍历
		//非递归 前序遍历
		void no_recursive_preorder(const DealKey& deal )const override
		{
			if( size() == 0 )
				return;
			std::vector<Node*> st(size());
			Node*node = root_;
			int top = 0;
			while( top>0 || node !=sentienl_ )
			{
				while( node != sentienl_ )
				{
					deal(node);
					st[top++] = node;
					node = node->left_;
				}
				if(top>0)
				{
					node = st[--top];
					node = node->right_;
				}
			}
		}
		//非递归遍历的 中序遍历
		void no_recursive_inorder(const DealKey& deal )const override
		{
			if( size() == 0 )
				return;
			std::vector<Node*> st(size());
			Node* node = root_;
			int top=0;
			while( top>0 || node !=sentienl_ )
			{
				while( node !=sentienl_ )
				{
					st[top++] = node;
					node = node->left_;
				}
				if(top>0)
				{
					node = st[--top];
					deal(node);
					node = node->right_;
				}
			}
		}
		//非递归遍历的 后续遍历 
		//这个比较麻烦，需要给节点打上标记
		//注明是要通过该节点找到右孩子 还是遍历完右孩子 回到该节点
		void no_recursive_postorder(const DealKey& deal )const override
		{
			if( size() == 0 )
				return ;
			std::vector<Node*>st(size());
			int top = 0;
			std::vector<uint8>flag(size(),0);
			Node* node = root_;
			while( top>0 || node !=sentienl_ )
			{
				while(node !=sentienl_ )
				{
					st[top++] = node;
					flag[top-1]=1;
					node = node->left_;
				}
				if( top>0 )
				{
					if( flag[top-1] == 1 )
					{
						node = st[top-1]->right_;
						flag[top-1]==2;
					}
					else if( flag[top-1] == 2 )//可以出栈
					{
						node = st[--top];
						deal(node);
						node = sentienl_;//这个元素一定要置空防止再次遍历
					}
				}
			}
		}
	private:
		void destruct(Node*node )
		{
			if(node!=sentienl_)
			{
				destruct(node->left_);
				destruct(node->right_);
				delete node;
			}
		}
		size_t height(Node* node )
		{
			if( node == sentienl_ )
				return 0;
			return std::max<size_t>( height(node->left_), height(node->right_))+1;
		}
		void _preorder(const DealKey& deal , Node* node )
		{
			if(node !=sentienl_ )
			{
				deal(node);
				_preorder(node->left_);
				_preorder(node->right_);
			}
		}
		void _inorder(const DealKey& deal, Node* node )
		{
			if( node!= sentienl_ )
			{
				_inorder(node->left_);
				deal(node);
				_inorder(node->right_);
			}
		}
		void _postorder(const DealKey& deal, Node* node )
		{
			if(node!=sentienl_ )
			{
				_postorder(node->left_);
				_postorder(node->right_);
				deal(node);
			}
		}
		Node*     root_;
		size_t    size_;
		Node*     sentienl_;//哨兵节点，根节点的父节点，叶节点子孩子都指向这个值
	};

}//namespace
#endif