#ifndef __Binary_Search_Tree__HPP
#define __Binary_Search_Tree__HPP
#include "stack.hpp"//非递归版的三种遍历
/*
	二叉查找树(二叉搜索树) 插入查找的效率最好的时候logn 最差n(比如一次插入 1 2 3 4 5 就会退化成链表

	每一个结点的值都不同，也就是整棵树中的每一个结点都拥有不同的值
	每一个结点的数据大于左子树结点（如果有的话）的数据，但是小于右子树的结点（如果有的话）的数据。
	左右两部分的子树，也是一课二叉查找树。

	注意创建的时候 或者插入的时候 由于会在下一层new一个对象, 如果用一级指针,那么上一次递归调用的指针
	跟当前函数的指针 是两个完全不一样的指针,如果仅给当前函数赋值,那么上一层的指针依旧是nullptr
	这个就跟
	fun( char*ptr )
	{
		ptr = (char*)malloc(N);
	}
	char*p = nullptr;
	fun(p)
	是一个道理,这里会造成内存泄漏,而且p指针依旧为nullptr
	也可以通过返回一个指针的方式
	但是我们的insert函数是不需要向外部展现node信息的 所以这里采用二级指针的方式
	当然 也可以采用指针的引用

	二叉树的很多操作,如果是一个方向的都可以写成简单的while循环的
	比如插入/查找
	如果涉及两个方向 就需要递归做,也可以借助其他数据结构做非递归版
	比如遍历

	树中的最大值,根结点右子树中的最右结点
	树中的最小值,根节点左子树中的最左结点

	删除二叉树的时候设计到两个概念 一个节点的前驱和后继
	前驱后继结点的定义是在中序遍历的结果中,当前结点的前一个结点和后一个结点
	前驱:小于当前结点中最大的结点
	后继:大于当前结点中最小的结点
	关于前驱后继可以这么理解:
				a
			b           c
		d      e     f      g
	如上 c是f的右父母 是g的左父母,可知右父母是大于当前结点的 左父母是小于当前结点的

	当前结点的值为X的话
	前驱:
		如果左子树存在,则前驱结点就是左子树中的最左结点,即左子树中最小结点
		左子树不存在,则从当前结点往上找,直到找到第一个有右子树的父节点,即为当前结点的前驱结点
	后继:
		如果右子树存在,则找出右子树中最小的
		否则从当前结点往上找出第一个有左子树的结点

	关于这一点的理解,可以想象中序遍历,中序遍历是左中右的顺序,
	如果有左右子树的情况下,左中右 则左边的最左结点就是当前结点的前驱
									右边的最右结点就是当前结点的后继
	如果没有子树 前驱结点就是从当前结点往上遍历第一个有右子树的结点(左中右 此时当前结点做了右,那么前驱不就是第一个有右子树的父节点)
				 后继结点就是从当前结点往上遍历第一个有左子树的结点(左中右 此时当前结点做了左,那么后继不就是第一个有左子树的父节点)
	前驱后继可以不需要父节点的时候也能找到
	具体做法是,在最开始查找数据的时候,不但记录下待求结点的父节点,
		同时如果是找前驱结点,则在查找路径上,记录最后一次有右孩子的父节点,
			如果找到的结点没有左子树,那么这个最后一个有右孩子的父节点就是他的前驱
		如果是找后继结点,则在查找路径上,记录最后一次有左孩子的父节点
			如果找到的结点没有右子树,那么这个最后一次有左孩子的父节点就是他的后继结点

	非递归的二叉树遍历
	前序中序都好理解
		前序/中序都是不停的一路往左下放遍历,并把结果放在栈内,即放入栈内的是根左
		所以:
		前序就是在入栈的时候访问,因为先访问根节点嘛,然后通过根节点访问最下面的右子树
		中序就是在出栈的时候访问,因为栈内的顺序是根左,先出来的是左结点,然后进入右子树
		可以认为左孩子结点是左子树的根节点,那么栈内存的就全是根节点
		当然了 也可以用数组模拟栈

	后序遍历的非递归遍历,比较麻烦,因为我们保存的各个树的根结点,前序 中序 因为右子树都是最后访问,所以可以在入栈的时候访问就是前序
	出栈的时候访问就是中序,但后序就不一样了,当访问到一个根节点,可能是准备去右结点 这时不能处理的
	必须等到右子树处理了,才能处理根节点
	入栈和前面的一样 栈里保存的从根节点往左的所有的树的根节点,如果最后的结点左子树没有值,右子树有值,那么先访问右子树
	即第一步通过栈内的结点获取右子树,然后判断右子树有没有其他结点,没有就出栈,然后需要将栈内的数据弹出
	这里根节点会访问三次 第一次正常遍历 第二次为了获取右结点,第三次才是需要真正操作的,所以需要一个额外的数组记录结点的访问状态
	1:第一次访问 2:第二次获取右结点 3:出栈(具体操作这里做)


	删除操作比较复杂:
		1:如果当前待删除的结点,是叶子结点,即没有左孩子结点也没有右孩子结点,那么直接删除即可,同时也得把父节点的相应指针域置空
		2:如果只有一个子节点,那么把父节点的相应的指针域赋值给子孩子结点即可,子孩子的父节点也改变成当前结点的父节点
		(父节点可以不要,后面实现一个查找前驱后继结点不需要父节点的版本)
		3:如果即有左孩子又有有孩子,则可以用当前结点的前驱或者后继来替换当前结点,然后把交换后的结点去掉即可
		由于前驱或者后继结点必然是只有一个子节点或者没有子节点(因为这次相当于查找的是 两个子树中的最大或者最小值
		这种情况下,必然不会出现最大或者最小的结点有两个子树的情况!!!因为最大有右孩子就不能保证最大,最小有左孩子就不能保证最小
		情况3可以转化成情况1 和情况2 所以写个非递归的版本,先处理情况3,然后将3转化成1 2 两种情况


	对整个二叉树进行中序遍历,在中序遍历的结果中,当前节点的前一个值 即它的前驱结点,后一个结点时它的后继结点
	但是这种的时间复杂度时O(n)

*/
/*
     BST树   AVL树 红黑树 本质上都是搜索树,所以这里实现接口类,基类不同的子类实现不同的函数
	 成员函数如果带模板参数,则不能做虚函数

*/

#include<vector>
template<typename Key,typename Value>
class BaseBinarySearchTree {
public:
	struct Node {
		Node *left_ = nullptr;
		Node *right_ = nullptr;
		//Node *parent_ = nullptr;//父节点 为了找前驱后继结点
		Key    key_;
		Value  val_;
		Node(const Key& key, const Value& val)
			:key_(key), val_(val), left_(nullptr), right_(nullptr) {}
	};
	~BaseBinarySearchTree() {}
    using DealKey = std::function<void(const Key&)>;
	
	//大小
	virtual  size_t size()const = 0;
	//二叉树的高度
	virtual  size_t height()const = 0;
	//最大
	virtual   const Value&  findmax()const = 0;
	//最小
	virtual   const Value&  findmin()const = 0;
	//前驱 后继
	virtual  bool    getprenode(const Key&key, Value& val)const = 0;
	virtual  bool    getnextnode(const Key&key, Value& val)const = 0;

	//查找
	virtual  bool    find(const Key&key, Value&val)const = 0;
    //插入 重复返回false
	virtual bool insert(const Key& key, const Value& val) = 0;
	//删除
	virtual bool deletenode(const Key&key) = 0;
	//三种遍历 递归和非递归版 第三个操作代表针对元素的操作,是个可调用对象
	virtual void preorder(const DealKey& fun)const =0;
	virtual void inorder(const DealKey& fun)const = 0;
	virtual void postorder(const DealKey& fun)const = 0;
	//非递归版的三种遍历
	virtual void no_recursive_preorder(const DealKey& fun)const = 0;
	virtual void no_recursive_inorder(const DealKey& fun) const = 0;
	virtual void no_recursive_postorder(const DealKey& fun)const = 0;
};

template<typename Key,typename Value>
class BinarySearchTree :public BaseBinarySearchTree <Key, Value>{
public:
    /* 坑爹啊 windows下正常编译 linux下就必须带这俩 不然编译不过 */
    /* 模板类中还有using 或者typedef 时 注意在子类中要用这俩类型 必须typename 指定类型*/
    using DealKey = typename BaseBinarySearchTree<Key,Value>::DealKey;
    using Node = typename BaseBinarySearchTree<Key,Value>::Node;
	BinarySearchTree():size_(0),root_(nullptr)
	{
	}
	~BinarySearchTree()
	{
		_destruct(root_);
		size_ = 0;
		root_ = nullptr;
	}
	//重复数据插入失败
	bool  insert(const Key& key, const Value& val)override
	{
		//return Insert(&root_, key, val);
		//不递归应该也可以
		Node* temp = root_, *parent = nullptr;;
		while (temp)
		{
			parent = temp;
			if (temp->key_ == key)
				return false;
			if (key<temp->key_)
				temp = temp->left_;
			else
				temp = temp->right_;		
		}
		if (parent == nullptr)//当前构造的结点时根节点
		{
			root_  = new Node(key, val);
		}
		else
		{
			if (parent->key_ > key)
				parent->left_ = new Node(key, val);
			else
				parent->right_ = new Node(key, val);
		}
		size_++;
		return true;
	}
	size_t size()const override
	{
		return size_;
	}
	size_t height()const override
	{
		return _height(root_);
	}
	bool find(const Key&key, Value&val )const override
	{
		//可以是非递归的写法
		Node* node = _find(root_, key);
		if (node != nullptr)
			val = node->val_;
		return node != nullptr;
	}
	//有父节点的情况下 求前驱结点
	bool getprenode(const Key& key, Value& val)const override
	{
#if 0
		//结点信息中包含父节点的时候 查找前驱结点
		//先找到这个结点
		Node* node = root_;
		while (node != nullptr && node->key_ != key)
		{
			if (key > node->key_)
				node = node->right_;
			else
				node = node->left_;
		}
		if (node == nullptr)//没找到 直接返回
			return false;
		//有左子树的情况,前驱就是左子树的最大值
		if (node->left_ != nullptr)
		{
			Node* temp = find_max(node->left_);
			if (temp == nullptr)//无前驱结点
				return false;
			val = temp->val_;
			return true;
		}
		else //没有左子树,则找到父节点中第一个拥有右子树的结点
		{
			Node*parent = node->parent_;
			while (parent && node != parent->right_)
			{
				node = parent;
				parent = parent->parent_;
			}
			if (parent == nullptr)
				return false;
			val = parent->val_;
			return true;
		}
#endif
		//结点信息中不包含父节点的时候的前驱结点查找
		//因为前驱结点是 有左子树的时候 左子树里面的最左结点,没有左子树时 从当前结点往上第一个拥有右子树的父节点
		//所以遍历的时候,由于是从上到下遍历,所以最后一个有右子树的父节点,即为从当前结点往上遍历的第一个有右子树的父节点
		Node* parent = nullptr, *lastNodeHaveRightChild = nullptr,*current=root_;

		while (current && current->key_ != key)
		{
			parent = current;
			if (key > current->key_)
			{
				current = current->right_;
				//搜索路径中 当前结点是父亲结点的右孩子,这里不能写在后面,不然不对的
				lastNodeHaveRightChild = parent;
			}
			else
				current = current->left_;		
		}
		if (current == nullptr)
			return false;
		Node* preNode = nullptr;
		if (current->left_)//左子树存在,左子树中的最大值 即左子树的最右值
			preNode = find_max(current->left_);
		else//否则就是从当前结点往上找的第一个含有右子树的父节点
			preNode = lastNodeHaveRightChild;
		if (preNode)
			val = preNode->val_;
		return preNode != nullptr;

	}
	//有父节点的情况下,求后继结点
	bool getnextnode(const Key& key, Value& val)const override
	{
#if 0
		//找到这个结点
		Node* node = root_;
		while (node && node->key_ != key)
		{
			if (key > node->key_)
				node = node->right_;
			else
				node = node->left_;
		}
		if (node == nullptr)//未找到
			return false;
		//存在右子树,右子树的最小值即结果
		if (node->right_)
		{
			Node* temp = find_min(node->right_);
			if (temp == nullptr)
				return false;
			val = temp->val_;
			return true;
		}
		else//从该结点往上找,找出第一个拥有左子树的结点
		{
			Node* parent = node->parent_;
			while (parent && node != parent->left_)
			{
				node = parent;
				parent = parent->parent_;
			}
			if (parent == nullptr)
				return false;
			val = parent->val_;
			return true;
		}
#endif
		//没有父节点的情况下,查找后继结点
		//同样 如果当前结点由右子树,则后继结点就是右子树的最右结点
		//如果没有就是从当前结点往上查找,直到找到一个当前结是父节点的左孩子(左中右的顺序,父节点就是它的后继结点)
		Node* parent = nullptr, *node = root_, *lastHaveLeftChild = nullptr;
		while (node && node->key_ != key)
		{
			parent = node;
			if (key < node->key_)
			{
				lastHaveLeftChild = parent;
				node = node->left_;
			}
			else
				node = node->right_;
		}
		if (node == nullptr)
			return false;
		Node* nextNode = nullptr;
		if (node->right_ != nullptr)//右子树中的最小值 即右子树的左值
			nextNode = find_min(node->right_);
		else
			nextNode = lastHaveLeftChild;
		if (nextNode)
			val = nextNode->val_;
		return nextNode != nullptr;
	}
	/*
	*/
	bool deletenode(const Key& key)override
	{
#if 0
		//包含父节点的删除操作
		Node* current = _find(root_, key);
		if (current == nullptr)//树中无此结点
			return false;
		Node* parent = current->parent_;
		//两个子节点都存在的情况
		if (current->left_ != nullptr && current->right_ != nullptr)
		{
			//找出current的前驱或者后继其中一个即可,这里我们找后继
			Node* temp = find_min(current->right_);
			//跟当前结点替换key value
			std::swap(temp->key_, current->key_);
			std::swap(temp->val_, current->val_);
			parent = temp->parent_;
			current = temp;//右子树的最小,必然最多只有一个孩子结点,就可以转化成下面的情况统一处理
		}
		Node* child = current->left_;
		if (child == nullptr)
			child = current->right_;

		if (current == root_)
		{
			root_ = child;
		}
		else
		{
			if (parent->left_ == current)//这里也包含对叶子结点的处理,即current == nullptr 
				parent->left_ = child;
			else
				parent->right_ = child;
		}
		if (child != nullptr)//左右结点为空的时候 不用关系子节点的父节点的指向
		{
			child->parent_ = parent;
		}
		delete current;
		size_--;
		return true;
		//结点为空
#endif
		//不包含父节点的删除操作
		Node*parent = nullptr, *current = root_;
		while (current&& current->key_ != key)
		{
			parent = current;
			if (key > current->key_)
				current = current->right_;
			else
				current = current->left_;
		}
		if (current == nullptr)
			return false;
		/*
			删除分三种情况
			第一种: 左子树右子树都有的 这种其实有两种做法,要么用他的前驱代替自己然后删除原来前驱的位置
			要么用后继代替自己然后删除原来后继的位置,这里注意 这里前驱后继其实就是左子树的最左结点和右子树的最右结点
			因为他们本身是左子树或者右子树最大或者最小 所以必然最大的没有右孩子结点 最小的没有左孩子结点,即这里的前驱最多
			只能有一个结点!! 只有一个或者没有结点可以转化成下面的做法
			第二种: 只有一个孩子结点
			这时将孩子结点赋值给父节点对应的指针域
			第三种:没有孩子结点 直接删除 然后把父节点对应的指针域置为空
		*/
		//左右都存在的
		if (current->left_ && current->right_)
		{
			//跟上面不一样 这里我们找前驱 但是还需要找出前驱结点的父节点
			Node* preNode = current->left_;
			//这里是找的左子树 这样的话parent就是current
			parent = current;
			while (preNode && preNode->right_ != nullptr)
			{
				parent = preNode;
				preNode = preNode->right_;
			}
			//替换本身的key value 这里的指针域暂时不动,留在后面做相应的修改
			std::swap(preNode->key_, current->key_);
			std::swap(preNode->val_, current->val_);
			//current替换成当前结点 parent上面也做了修改 继续删除这个结点,以为这个结点最多只有一个孩子,可以跟下面的统一起来
			current = preNode;
		}
		Node* child = current->left_;
		if (child == nullptr)
			child = current->right_;
		//child 为空或者 仅有 一个孩子的情况 这里需要修改父节点了,但是当前结点可能会是根节点
		if (current == root_)
		{
			root_ = child;//root结点不用管父节点
		}
		else
		{
			//修改对应的指针域
			if (parent->left_ == current)
				parent->left_ = child;
			else
				parent->right_ = child;
		}
		//全部处理完了 delete 
		delete current;
		size_--;
		return true;
	}
	//三种遍历 返回遍历后的数组
	void preorder(const DealKey& fun )const override
	{
		//前序遍历
		_preorder(fun, root_);
	}
	//中序遍历会使key有序
	void inorder(const DealKey& fun)const override
	{
		_inorder(fun, root_);
	}
	//后序
	void postorder(const DealKey& fun)const override
	{
		_postorder(fun, root_);
	}
	//非递归版的三种遍历方式 需要借助数据结构stack
	void no_recursive_preorder(const DealKey& fun)const override
	{
		if (size() == 0)
			return;
#if 0
		//前序遍历的非递归版本 跟左右
		ArrayStack<Node*> st(size());
		st.Push(root_);
		Node*node;
		while (!st.IsEmpty())
		{
			node =st.Top();
			st.Pop();
			fun(node->key_);//这里因为时根左右 所以先把右孩子放到栈里面
			if (node->right_)
				st.Push(node->right_);
			if (node->left_)
				st.Push(node->left_);
		}
#endif 
		//第二种写法,上面的写法 我觉得挺简单的 
		Node* node = root_;
		ArrayStack<Node*> st(size());
		while (node || !st.IsEmpty())
		{
			//根左右 先访问根 在访问左子树,边遍历遍历,一次完事node结点的所有根结点左结点全部遍历完
			//push是为了访问右孩子
			while (node)
			{
				fun(node->key_);
				st.Push(node);//用于先放问左子树
				node = node->left_;
			}
			if (!st.IsEmpty())
			{
				node = st.Top();//这个node上面已经遍历过了
				st.Pop();//进入最下面结点的右子树 从下往上进入每个结点的右子树 右子树遍历也遵循根左右
				node = node->right_;
			}
		}
	
	}
	//非递归版的三种遍历方式 需要借助数据结构stack
	void no_recursive_inorder(const DealKey& fun)const override
	{
#if 0
		//中序遍历 左根右
		Node* node = root_;
		ArrayStack<Node*> st(size());
		while (node || !st.IsEmpty())
		{
			while (node)
			{
				st.Push(node);//此时 栈里面保存的就 根->左 按顺序弹出就是左根的顺序
				node = node->left_;
			}
			//当左子树为空的时候里面 存的是根结点 弹出来就是左根的顺序 再探查一下右子树,也按照左根右的顺序
			if (!st.IsEmpty())
			{
				node = st.Top();
				st.Pop();
				fun(node->key_);
				node = node->right_;//假设右结点为null下次就会弹出上一个的左子树
			}
		}
#endif
		//通过数组模拟栈
		if (size() == 0)
			return;
		std::vector<Node*> st(size(), nullptr);
		int top = 0;
		Node* node = root_;
		while (top > 0 || node)
		{
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
	//非递归版的三种遍历方式 需要借助数据结构stack
	void no_recursive_postorder(const DealKey& fun)const override
	{
		//用数组模拟栈 是为了方便记录每个元素的状态
		std::vector<Node*> st(size(), nullptr);
		int top = 0;
		std::vector<uint8_t> flags(size(), 0);
		Node* node = root_;
		while (node || top>0 )
		{
			while (node)
			{
				st[top++] = node;
				node = node->left_;
				//这里记录node结点的状态 即这是首次访问,数组下标是top-1
				flags[top - 1] = 1;
			}
			if (top > 0)
			{
				if (flags[top - 1] == 1)//此时应该要访问右结点,而不是出栈
				{
					node = st[top - 1];
					node = node->right_;
					flags[top - 1] = 2;//这时需要记录下他的状态 即已经访问完右结点
				}
				else//此时就该出栈了
				{
					node = st[--top];
					fun(node->key_);
					//这时候需要继续弹栈  所以这个node需要置为空
					node = nullptr;
				}
			}
		}
	}
	//树中最大值 即右子树中的最右结点
	const Value& findmax()const override
	{
		static Value default_val_ = Value();
		Node *node = find_max(root_);
		return node == nullptr ? default_val_ : node->val_;
	}
	//最小 即最左边的
	const Value& findmin()const override
	{
		static Value default_val_ = Value();
		Node *node = find_min(root_);
		return node == nullptr ? default_val_ : node->val_;
	}
private:
	bool _insert(Node*& node, const Key& key, const Value& val)
	{
		if (node == nullptr)
		{
			node = new Node(key, val);
			size_++;
			return true;
		}
		if (node->key_ == key)//已经有 覆盖
		{
			node->val_ = val;
			return false;
		}
		else if (node->val_ > val)
			return _insert(node->left_, val);
		else
			return _insert(node->right_, val);
	}
	//递归版本
	bool _find(Node* node, const Key&key, Value&val)
	{
		if (node == nullptr)
			return false;//未找到

		if (node->key_ == key)
		{
			val = node->val_;
			return true;
		}
		if (key > node->key_)
			return _find(node->right_, key, val);
		else
			return _find(node->left_, key, val);
	}
	Node* _find(Node*node, const Key&key)const //以node为根节点的树上找到key的结点
	{
		while (node && node->key_ != key)
		{
			if (key > node->key_)
				node = node->right_;
			else
				node = node->left_;
		}
		return node;
	}
	void _destruct(Node* temp)
	{
		if (temp)
		{
			_destruct(temp->left_);
			_destruct(temp->right_);
			delete temp;
		}
    }
	void _preorder(const DealKey& fun, Node*node)const
	{
		if (node == nullptr)
			return;
		fun(node->key_);
		_preorder(fun, node->left_);
		_preorder(fun, node->right_);
	}
	void _inorder(const DealKey& fun, Node*node)const
	{
		if (node == nullptr)
			return;
		_inorder(fun, node->left_);
		fun(node->key_);
		_inorder(fun, node->right_);
	}
	void _postorder(const DealKey& fun, Node* node)const 
	{
		if (node == nullptr)
			return;
		_postorder(fun, node->left_);
		_postorder(fun, node->right_);
		fun(node->key_);
	}
	Node* find_max(Node*node)const //找出以node为根的树里面最大的
	{
		while (node && node->right_ != nullptr)
			node = node->right_;
		return node;
	}
	Node* find_min(Node*node)const //找出以node为根的树里最小的
	{
		while (node && node->left_)
			node = node->left_;
		return node;
	}
	size_t _height(Node*node)const
	{
		if (node == nullptr)
			return 0;
		return std::max<size_t>(_height(node->left_), _height(node->right_)) + 1;
	}
	size_t     size_ = 0;
	Node       *root_;
};
#endif
