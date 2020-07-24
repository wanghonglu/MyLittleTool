#ifndef  __AVL_TREE_HPP
#define  __AVL_TREE_HPP
/*	
	前面的BST树有局限性 比如如果插入的数据时按顺序插入1-1000万数据  那么树就会变成只有右子树的树了 类似链表了
	查找 插入的复杂度就变成O(n) 跟普通的链表一样了,所以有了自平衡的二叉树 AVL树
	AVL树的必要条件是:
		1:必须是一颗二叉查找树
		2:每个结点的左右子树高度差至多为1

	要维护AVL树的高度差(也叫高度因子)绝对值不超过1,需要在插入和删除的时候对二叉树进行旋转
	插入的时候分为四种情况旋转
		3								3					   3						  4
	2		4						2         4            2		5				3			5			
				5				1										8		1					
					6	    0										7				2					
	 (左旋转)					(右旋转)			    (先右后左)					(先左后右)
	这个其实很绕,绕也很正常
	1: 左旋转
		当右子树插入右孩子导致AVL失衡时
		旋转方法为,旋转离插入结点最近的失衡结点, 失衡结点的右孩子如果有左子树,该左子树变成失衡结点的右子树
		失衡结点变成右子树的左孩子
		如图 4结点失衡 5结点没有左子树,所以直接把4变成5的左子树
		因为4要做5的左子树,那么5的左子树只能给4,因为5的左子树比4大,所以只能左4的右子树
		具体原理想想就能想明白
		调整后变成
					3
			2			   5
					4				6
	2: 右旋转
		当左子树插入左孩子导致AVL失衡时(当插入0时)
		旋转方法为,旋转离插入结点最近的失衡结点,  失衡结点的左孩子如果有右子树,该右子树变成失衡结点的左子树
		失衡结点变成他左孩子的右子树
		如果 2结点失衡 1结点没有右子树,直接把2结点变成1的右子树
		这个也同样,失衡节点要做左孩子的右子树,原来左孩子的右子树只能给失衡节点,因为都比失衡节点小,所以只能做左子树
		调整后变成
				  3
			1			4
        0        2 
	3: 先右旋后左旋
	   当右子树插入左孩子导致AVL不平衡,需要先右旋再左旋
	   旋转方法为,旋转里插入结点最近的失衡结点,对失衡结点的右孩子右旋转,再对失衡结点左旋
	   注意这里是先对失衡结点的右孩子进行右转,再对本身进行左旋,两次旋转的点都不一样 
	   比如5结点失衡,对8结点右旋,7没有右子树,所以直接将8变成7的右子树
	   得到
	   2
3			5
				7
					8
	   然后对5结点左旋,7的左子树变成5的右子树,5变成7的左子树
	   变成
	   2
3			7
		5        8 
		平衡了
	4: 先左旋后右旋
		当在左子树,插入右孩子的时候导致AVL不平衡,需要先左旋后右旋
		旋转方法为,对失衡结点的左孩子进行左旋,然后对失衡结点进行右旋
		这里先是对失衡结点的左孩子左旋,然后对失衡结点右旋
		如图三号结点失衡,对三号结点的左孩子左旋得到
		2号结点的左孩子变成1号结点的右孩子,然后1号结点变成2号结点的左孩子
		变成		
			4
		3        5
	2
1
	    然后对三号结点进行右旋变成
		2号结点的右孩子变成3号结点的左孩子,3号结点变成2号结点的右孩子
		  4
	2			5
1		3
		总结一下就是四种
		单左旋:	右子树插入右孩子   把右孩子的左子树给结点的右子树,然后把当前结点变成右孩子结点的左子树
		单右旋: 左子树插入左孩子   把左孩子的右子树给结点的左子树,然后把当前结点变成左孩子的右子树
		先左后右旋:左子树插入右孩子  把左孩子左选,然后把当前结点右旋
		先右后左旋:右子树插入左孩子  把右孩子右旋,然后把当前结点左旋

	删除也很麻烦
		删除的时候 BST在左右子树都存在的时候,可以要么取左子树的最大 要么取右子树的最小 即可以拿前驱也可以拿后继代替自己然后删除
		但是AVL会选择其中一个更长的删除,尽量减少调整的可能
		删除右子树的结点导致失衡,可以看成在未删除的树上添加了结点,这样等价于插入,然后按照情况 进行上面四种旋转即可
	
*/	

#include "BinarySearchTree.hpp"

namespace datastruct {
template<typename Key,typename Value>
class AVLTree :public BaseBinarySearchTree<Key,Value>
	{
#define GetHeight(node) ((node)?node->height_:0)
		struct Node {
			Key		key_;
			Value	val_;
			Node*   left_;
			Node*   right_;
			size_t  height_=0;//树高
			Node(const Key& key, const Value& val) :key_(key), val_(val), left_(nullptr), right_(nullptr), height_(0){}
		};
	public:
		//类的类型成员跟静态成员一样属于类本身,不属于对象,模板类父类中的类型成员,子类要用typename 申明之后再用
		using DealKey = typename BaseBinarySearchTree<Key, Value>::DealKey;
		AVLTree();
		~AVLTree();
		//大小
		size_t size()const override;
		//二叉树高度
		size_t height()const override;
		//最大 最小
		const Value& findmax()const override;
		const Value& findmin()const override;
		//前驱后继
		bool  getprenode(const Key&key, Value& val)const override;
		bool  getnextnode(const Key&key, Value& val)const override;
		//查找
		bool  find(const Key&key, Value& val)const override;
		//插入   默认都成功 重复就覆盖
		bool  insert(const Key& key, const Value& val)override;
		//删除 不存在不报错
		bool  deletenode(const Key&key)override;
		//遍历 前序 中序 后续 以及他们的非递归形式
		void preorder(const DealKey& fun)const override;
		void inorder(const DealKey& fun)const override;
		void postorder(const DealKey& fun)const override;
		void no_recursive_preorder(const DealKey& fun)const override;
		void no_recursive_inorder(const DealKey& fun)const override;
		void no_recursive_postorder(const DealKey& fun)const override;
 		//层序遍历
		void level_order(const DealKey& fun)const override;
		//检测是不是平衡 每个结点的左右子树高度差不超过1
		bool is_balance()const;
		//下标访问
		Value& operator [](const Key&key);
	private:
		//平衡因子 左右子树的高度差的绝对值
		inline size_t getbalancefactor(Node*);
		void destruct(Node* node);

		//最左结点
		Node* find_min(Node*);
		//最右结点
		Node* find_max(Node*);

		bool is_balance(Node*node);

		preorder(const DealKey&fun, Node*);

		inorder(const DealKey&fun, Node*);

		postorder(const DealKey&fun, Node*);
		//四种旋转
		//单左旋
		//插入
		Node* insert(Node*, const Key& key, const Value& val)
			//删除操作 可以认为是变相的插入操作
		Node* deletenode(Node*, const Key&key, const Value& val);
		Node* 
		Node* leftRotate(Node*);
		//单右旋
		Node* rightRotate(Node*);
		//先左旋再右旋
		Node* leftAndRightRotate(Node*);
		//先右旋再左旋
		Node* rightAndLeftRotate(Node*);
		Node	*root_;
		size_t	size_;
	};
template<typename Key,typename Value>
AVLTree<Key, Value>::AVLTree() :root_(nullptr), size_(0)
{
}
template<typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree()
{
	destruct(root_);
	root_ = nullptr;
	size_ = 0;
}
template<typename Key, typename Value>
size_t AVLTree<Key, Value>::size()const
{
	return size_;
}
template<typename Key, typename Value>
size_t AVLTree<Key, Value>::height()const
{
	return root_ ? root_->height_ : 0;
}
//平衡因子 左右子树高度差
template<typename Key, typename Value>
size_t AVLTree<Key, Value>::getbalancefactor( Node* node)const 
{
	if (node == nullptr)
		return 0;
	int lhight = node->left_ ? node->left_->height_ : 0;
	int rhight = node->right_ ? node->right_->height_ : 0;
	return lhight > rhight ? lhight - rhight : rhight - lhight;
}
template<typename Key, typename Value>
bool AVLTree<Key, Value>::is_balance()const
{
	return is_balance(root_);
}
template<typename Key, typename Value>
bool AVLTree<Key, Value>::is_balance(Node*node)const
{
	if (getbalancefactor(node) > 1)
		return false;
	return getbalancefactor(node->left_) && getbalancefactor(node->right_);
}
template<typename Key,typename Value>
Node* AVLTree<Key, Value>::find_min(Node* node )
{
	while (node && node->left_)
		node = node->left_;
	return node;
}
template<typename Key, typename Value>
Node* AVLTree<Key, Value>::find_max(Node* node)
{
	while (node && node->right_)
		node = node->right_;
	return node;
}
template<typename Key, typename Value >
const Value& AVLTree<Key, Value>::findmax()const
{
	static Value default_val_ = Value();
	Node* node = find_max(root_);
	return node ? node->val_ : default_val_;
}
template<typename Key, typename Value >
const Value& AVLTree<Key, Value>::findmin()const
{
	static Value default_val_ = Value();
	Node* node = find_min(root_);
	return node ? node->val_ : default_val_;
}
//查找
template<typename Key, typename Value >
bool AVLTree<Key, Value>::find(const Key&key, Value& val)
{
	Node *node = root_;
	while (node && node->key_ != key)
	{
		if (key > node->key_)
			node = node->right_;
		else
			node = node->left_;
	}
	if (node)
		val = node->val_;
	return node != nullptr;
}
template<typename Key,typename Value>
Value& AVLTree<Key, Value>::operator[](const Key&key)
{
	static Value defaut_value = Value();
	Node* node = root_;
	while (node && node->key_ != key)
	{
		if (key > node->key_)
			node = node->right_;
		else
			node = node->left_;
	}
	return node ? node->val_ : defaut_value;
}
//前驱
//左子树存在 前驱就是左子树中最大的 否则就是从当前结点往上第一个拥有右孩子
template<typename Key,typename Value>
bool AVLTree<Key, Value>::getprenode(const Key&key, Value& val)
{
	Node* node = root_, *lastHaveRightChild = nullptr;
	while (node && node->key_ != key)
	{
		if (key > node->key_)
		{
			lastHaveRightChild = node;
			node->right_;
		}
		else
			node->left_;
	}

	if (node == nullptr)
		return false;
	//有左子树 前驱即左子树中最大的
	// 否则 就是从他往上找第一个有右子树的结点 这个右子树结点在他的搜索路径上

	Node *prenode = nullptr;
	if (node->left_)
		prenode = find_max(node->left_);
	else
		prenode = lastHaveRightChild;
	if (prenode)
		val = prenode->val_;
	return prenode != nullptr;
}
//后继
//有右子树 右子树最小即他的后继 否则从该结点往上第一个有左孩子的父节点即他的后继
template<typename Key,typename Value>
bool AVLTree<Key, Value>::getnextnode(const Key&key, Value& val)
{
	Node* node = root_, *lastHaveleftChild = nullptr;
	while (node && node->key_ != key)
	{
		if (key > node->key_)
			node = node->right_;
		else
		{
			lastHaveleftChild = node;
			node = node->left_;
		}
	}
	if (!node)
		return false;
	//右子树存在 后继结点即右子树最小(最左)
	//否则是他的搜索路径上,从他往上第一个由左子树的结点
	Node *nextnode = nullptr;

	if (node->right_)
		nextnode = find_min(node->right_);
	else
		nextnode = lastHaveleftChild;
	if (nextnode)
		val = nextnode->val_;
	return nextnode != nullptr;
}
//遍历 递归形式
template<typename Key,typename Value>
void AVLTree<Key, Value>::preorder(const DealKey& fun,Node* node )
{
	if (node == nullptr)
		return;
	fun(node->key_);
	preorder(fun,node->left_);
	preorder(fun,node->right_);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::inorder(const DealKey& fun, Node* node)
{
	if (node == nullptr)
		return;
	inorder(fun, node->left_);
	fun(node->key_);
	inorder(fun, node->right_);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::postorder(const DealKey& fun, Node* node)
{
	if (node == nullptr)
		return;
	postorder(fun,node->left_);
	postorder(fun,node->right_);
	fun(node->key_);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::preorder(const DealKey& fun)
{
	preorder(fun, root_);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::inorder(const DealKey& fun)
{
	inorder(fun, root_);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::postorder(const DealKey& fun)
{
	postorder(fun, root_);
}
//遍历的非递归形式
template<typename Key,typename Value>
void AVLTree<Key, Value>::no_recursive_preorder(const DealKey& fun)
{
	//这里用数组模拟栈
	std::vector<Node*> st(size());
	int top = 0;
	Node*node = root_;
	while (node || top > 0)
	{
		while (node)
		{
			//先序遍历 先遍历的根结点,存在栈里是为了访问右子树
			fun(node->key_);
			st[top++] = node;
			node = node->left_;
		}
		if (top != 0)
		{
			//弹栈 此时应该进入右子树 右子树也满足根左右的遍历顺序
			node = st[--top];
			node = node->right_;
		}
	}
}
//非递归的中序遍历
template<typename Key,typename Value>
void AVLTree<Key, Value>::no_recursive_inorder(const DealKey& fun)
{
	std::vector<Node*> st(size());
	int top = 0;
	Node* node = root_;
	while (node || top > 0)
	{
		//左根右 所以在弹出栈的时候再遍历
		while (node)
		{
			st[top++] = node;
			node = node->left_;
		}
		if (top > 0)
		{
			node = st[--top];
			fun(node->key_);
			node = node->right_;
		}
	}
}
//非递归的后续遍历比较麻烦
//结点存入栈中都是为了访问右孩子,但是后序遍历是左根右 即要先访问左,再访问右最后访问根
//所以第一次存的时候记录状态1 第二次取出来访问右子树的时候记录1 第三次取的时候 是真正的该去掉的时候
template<typename Key,typename Value>
void AVLTree<Key, Value>::no_recursive_postorder(const DealKey& fun)
{
	std::vector<Node*> st(size());
	int top = 0;
	Node* node = root_;
	std::vector<uint8_t> flags(size());
	while (node || top > 0)
	{
		while (node)
		{
			st[top++] = node;
			flags[top - 1] = 1;
		}
		if (top > 0)
		{
			if (flags[top - 1] == 1)//第一次访问 这时是为了访问右子树
			{
				node = st[top - 1];
				node = node->right_;
				flags[top - 1] = 2;
			}
			else
			{
				node = st[--top];
				fun(node->key_);//右子树也遍历完了
				//此时弹栈 这个时候需要的是继续弹栈
				node = nullptr;
			}
		}

	}
}
//层序遍历
template<typename Key,typename Value>
void AVLTree<Key, Value>::level_order(const DealKey& fun)
{
	if (root_ == nullptr)
		return;
	LinkedListQueue<Node*>q;
	Node* node;
	q.push(root_);
	while (!q.empty())
	{
		node = q.front();
		fun(node->key_);
		q.pop();
		if (node->left_)
			q.push(node->left_);
		if (node->right_)
			q.push(node->right_);
	}
}
//四种旋转
//左旋 单左旋,往右子树插入右孩子导致的不平衡
//node结点的右孩子如果有左子树,变成node结点的右孩子
//node结点变成右孩子的左孩子
//返回旋转后的右孩子作为上面结点的新右孩子,
template<typename Key,typename Value>
Node* AVLTree<Key, Value>::leftRotate(Node*node)
{
	Node* n = node->right_;
	node->right_ = n->left_;
	n->left_ = node;

	//这里要重新设置他俩的高度
	node->height_ = std::max<size_t>(GetHeight(node->left_), GetHeight(node->right_)) + 1;
	n->height_ = std::max<size_t>(GetHeight(n->left_), GetHeight(n->right_)) + 1;
	return n;
}
//单右旋
/*	
	当在左子树插入左孩子 就需要单右旋
	将node结点的左孩子的右子树赋值给node结点的左孩子
	把node结点赋值给左孩子的右子树
	返回新的node结点的左孩子
*/
template<typename Key,typename Value>
void AVLTree<Key, Value>::rightRotate(Node* node)
{
	Node* n = node->left_;
	node->left_ = n->right_;
	n->right_ = node;
	//重新整理树高

	node->height_ = std::max<size_t>(GetHeight(node->right_), GetHeight(node->left_)) + 1;
	n->height_ = std::max<size_t>(GetHeight(n->left_), GetHeight(n->right_)) + 1;
	return n;
}

/*
	先右后左旋转
	当在右子树 插入左孩子的时候先右旋 再左旋
	此时先将node结点的右孩子左旋,然后再把node结点左旋
*/
template<typename Key,typename Value>
Node* AVLTree<Key, Value>::rightAndLeftRotate(Node*node)
{
	node->right_ = rightRotate(node->right_);
	return leftRotate(node);
}
/*
	先左后右旋转
	当左子树插入右结点的时候 此时先将node结点的左孩子左旋 然后把node结点右旋
*/
template<typename Key,typename Value>
Node* AVLTree<Key, Value>::leftAndRightRotate(Node*node)
{
	node->left_ = leftRotate(node->left_);
	return rightRotate(node);
}
/*
	BST树的插入可以通过 循环的方式进行,但是AVL的插入就不行了
	因为AVL要在回退的过程中判断那里的左右子树高度差超过1了,这种用循环不太好做
	就是AVL树针对递归的结果,要做相应的处理,所以不能循环只能递归
*/
template<typename Key,typename Value>
Node* AVLTree<Key, Value>::insert(Node* node, const Key&key, const Value& val)
{
	if (node == nullptr)//找到位置插入
	{
		node = new Node(key, val);//新建结点高度也设置为1
	}
	if (key == node->key_)//插入的结点 已经存在,修改一下
	{
		node->val_ = val;
	}
	else if (key > node->key_)//右子树插入
	{
		node->right_ = insert(node->right_, key, val);
		//平衡因子失衡
		if (getbalancefactor(node) > 1)
		{
			//这里需要判断是在右子树插入左孩子还是右孩子导致的失衡
			if (key > node->right_->key_)//右子树插入右结点左旋转
				node = leftRotate(node);
			else//右子树插入左孩子  先右旋再左旋
				node = rightAndLeftRotate(node);
		}
	}
	else//左子树插入
	{
		node->left_ = insert(node->left_, key, val);
		//回溯的过程中只有其中一个父节点会失衡 其他应该不用回溯
		if (getbalancefactor(node) > 1)
		{
			if (key > node->left_)//左子树插入右孩子先左旋再右旋
				node = leftAndRightRotate(node);
			else//左子树插入左孩子 右旋
				node = rightRotate(node);

		}
	}
	//所有操作做完,因为下面的一个结点的更新 上面的每个结点的高度也要更新
	node->height_ = std::max<size_t>(GetHeight(node->left_), GetHeight(node->right_)) + 1;
	return node;
}
template<typename Key,typename Value>
bool AVLTree<Key, Value>::insert(const Key& key, const Value& val)
{
	root_ = insert(root_, key, val);
	return true;
}
template<typename Key, typename Value>
bool AVLTree<Key, Value>::deletenode(const Key&key, const Value& val)
{
	root_ = deletenode(root_, key, val);
	return true;
}
/*
	删除操作 可以变相的认为是插入
	删除不存在不报错
*/
template<typename Key, typename Value>
Node* AVLTree<Key, Value>::deletenode(Node* node, const Key&key, const Value& val)
{
	if (node == nullptr)//没找到 返回
		return nullptr;
	if (key > node->key_)
		node->right_ = deletenode(node->right_);
	else if (key < node->key)
		node->left_ = deletenode(node->left_);
	else
	{
		//找到了就删除,这里和BST树的删除不一样 
		//BST树删除的时候 左右子树都存在的情况下,替换左子树最大或者右子树最小 都行然后删除
		//但这里为了使树尽量少的旋转,在左右子树中更高的一个树里面去删除
		Node* temp = nullptr;
		if (node->left_ != nullptr && node->right_ != nullptr)//左右子树都存在的情况
		{
			if (node->left_->height_ > node->right_->height_)//左子树比右子树高
			{
				temp = find_max(node->left_);//取左子树中最大的
				std::swap(node->key_, temp->key_);
				std::swap(node->val_, temp->val_);
				node->left_ = deletenode(node->left_, key, val);
			}
			else
			{
				temp = find_min(node->right_);//取右子树中最小的
				std::swap(node->key_, temp->key_);
				std::swap(node->val_, temp->val_);
				node->right_ = deletenode(node->right_, key, val);
			}	
		}
		else if (node->left_ != nullptr)//左子树不为空 右子树为空的情况
		{
			temp = node->left_;
			delete node;
			node = temp;
		}
		else if (node->right_ != nullptr)//左子树为空 右子树不为空
		{
			temp = node->right_;
			delete node;
			node = temp;
		}
		else//左右子树都为空的情况 直接删除返回即可
		{
			delete node;
			return nullptr;
		}
	}
	//需要重新node结点的高度,看看是否需要旋转
	node->height_ = std::max<size_t>(GetHeight(node->left_), GetHeight(node->right)) + 1;
	if (getbalancefactor(node) < 2)
		return node;//不需要调整
	else
	{
		//需要判断左右子树的高度
		//左子树比右子树高
		if (GetHeight(node->left_) > GetHeight(node->right_))
		{
			//左子树插入左孩子 右旋
			if (GetHeight(node->left_->left_) > GetHeight(node->left_->right_))
				node = rightRotate(node);
			else//左子树插入右孩子 先左旋再右旋
				node = leftAndRightRotate(node);
		}
		else//右子树比左子树高
		{
			//右子树插入左孩子 先右旋再左旋
			if (GetHeight(node->right_->left_) > GetHeight(node->right_->right_))
				node = rightAndLeftRotate(node);
			else//右子树插入右孩子 左选
				node = leftRotate(node);
		}
	}
	return node;
}
}//namespace 
#endif