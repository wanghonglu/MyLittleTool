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
		3: 左右  父节点是祖父结点的左孩子,当前结点是父节点的右孩子
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

			首先要找到删除节点的替换节点 而且如果有两个孩子的节点,真正删除的节点就不是该node节点了
			1:删除的节点没有孩子,则该节点直接删除即可,用它的左孩子或者右孩子作为调整节点去调整整个红黑树
			2:删除的节点有一个孩子,这时就用这个孩子为调整节点去调整整个红黑树
			3:删除的节点有两个孩子,找出右子树的最小作为替换该节点的删除节点,然后用该节点的右孩子去作为调整节点调整红黑树
			这个情况必然只有一个孩子,因为他是右子树最小,则必然不会有左孩子

			这里有一个问题,如果是情况1和情况2 则调整节点的父节点就变成原来删除的父节点
			但是3比较特殊,3的删除节点会转移,如果直接用转移后的父节点代替调整节点的父节点,但是有一种情况
			原节点是替身节点的父节点,调整节点的父节点就不能设置为替身节点的父节点,因为父节点会detele,会变成野指针

		找到这个调整起点,就开始平衡
			1:如果原来删除的节点是红色,则可以不用调整,因为不影响红黑树的任何性质
			2:如果这个起点节点是根节点,也不用调整直接变黑即可
		如果删除的节点是黑色,则有一定的难度,为了方便理解,把这个调整节点再附着一层黑色(删除节点如果红色,不用调整,则必然删除节点是黑色)
		这样依赖就违背了每个节点只能是红色或者黑色的原则
		这里只有这个调整节点是黑+黑时才需要调整,红+黑直接变色即可
		现在的矛盾是删除节点所在的子树少了一个黑色节点!!!
		当前节点时黑+黑,有四种方式调整
			1:兄弟节点是红色,则此时父节点,兄弟节点的孩子节点都是黑色
				 a:将兄弟节点变成黑色
				 b:父节点变成红色
				 c:父节点左旋
				 d:左旋后重新设置调整节点的兄弟节点
				 (大写是黑 小写是红)
				 P                           B
			N		 b        ->		p        BR
				  BL    BR			N		BL 
				 (调整节点兄弟节点变成了黑色,成了(原来兄弟节点的其中一个孩子) ) ,兄弟节点是黑色是后面的情况
				 此时红黑树的性质没有破坏,但N的兄弟节点变成了黑色

		    2:兄弟节点是黑色,且两个节点都是黑色 父节点可以是任何颜色
				  a:设置兄弟为红色
				  b:设置当前父节点为调整节点

				  父节点可以为任何颜色
				  (p)	                  (p)
			 N          B         ->	N       b
			 		SL     SR				BL      BR
				  这个处理还是好理解的,因为右子树也少了一个黑色节点,跟左子树平衡了,现在父节点变成了 原来的颜色+黑了
				  等于往上转移了

			3:兄弟节点是黑色,兄弟节点的左孩子是红色,右孩子是黑色(这里判断的时候 一定要写右孩子是黑色 而不是左孩子为红色,因为左右孩子都为红的情况留在下一个case处理)
				   a:设置兄弟节点的左孩子为黑色	 
				   b:设置兄弟节点为红色
				   c:对兄弟节点右旋
				   d:重新设置调整节点的兄弟节点
				
					这样的调整是为了后面,且此时也不破坏红黑树的性质
				(p)						(p)
			N		B         ->	N		BL
				 bl		BR  					b
				 									BR
			4:兄弟节点是黑色,兄弟节点的右孩子是红色,左孩子为任意色
					a:把父节点的颜色赋值给兄弟节点
					b:把父节点变黑
					c:把兄弟节点的右孩子设置为黑色
					d:对父节点左旋
					e:把调整节点设置为根节点( 这一步实际上是跳出循环)

					这一步很关键,也比较难
				(p)                     (b)
			N		B        ->		P		 BR  跳出循环   	
				 (bl) br		N      (bl)
					刚开始是p的左子树少了一个黑节点
					通过左旋让左子树多了一个黑节点(此时父节点由于拷贝了原来的父节点,所以颜色不变)
					左子树平衡了 右子树,原来有一个B的黑节点现在换成了BR,BR的树原来就是平衡的所以这里可以结束了
			这是当前节点是父节点的左孩子的情况,右孩子类似
		红黑树 ngnix实现没有注释 不太好读
		内核实现非常好 居然有注释
		https://github.com/torvalds/linux/blob/master/lib/rbtree.c
*/
#include "queue.hpp"
#include <vector>
namespace datastruct {
	#define NodeIsBlack(node)  (node)->color_==COLOR_BLACK
	#define NodeIsRed(node)	   (node)->color_ == COLOR_RED
	#define SetNodeBlack(node) (node)->color_ = COLOR_BLACK
	#define SetNodeRed(node)   (node)->color_ = COLOR_RED
	template<typename Key,typename Value>
	class RBTree :public BaseBinarySearchTree<Key, Value> {
		enum NodeColor{
			COLOR_RED,
			COLOR_BLACK,
		};
		struct Node {
			Key				key_;
			Value			val_;
			Node*			left_;
			Node*			right_;
			Node*			parent_;
			NodeColor		color_;
			explicit Node(const Key& key, const Value& val, Node* parent) :key_(key), val_(val), left_(nullptr),
				right_(nullptr), parent_(parent), color_(COLOR_RED) {}
		};
	public:
		RBTree() :root_(nullptr), size_(0)
		{
			sentienl_ = new Node(Key(), Value(), nullptr );
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
			Node* node = root_, *firstHaveRightChild = nullptr;
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
		bool getnextnode(const Key& key, Value& val )const override
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
			_preorder(deal,root_);
		}
		void inorder(const DealKey& deal )const override
		{
			_inorder(deal,root_);
		}
		void postorder(const DealKey& deal )const override
		{
			_postorder(deal,root_);
		}
		//层序遍历
		void level_order(const DealKey& deal )const override
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
				deal(node->key_);
				if( node->left_!=sentienl_)
					q.push(node->left_);
				if( node->right_ != sentienl_ )
					q.push( node->right_);
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
					deal(node->key_);
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
					deal(node->key_);
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
			std::vector<unsigned char>flag(size(),0);
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
						deal(node->key_);
						node = sentienl_;//这个元素一定要置空防止再次遍历
					}
				}
			}
		}
		//查找
		bool find(const Key& key, Value& val )const override
		{
			Node* node = root_;
			while( node !=sentienl_ && node->key_ != key )
			{
				if( key>node->key_ )
					node = node->right_;
				else
					node = node->left_;
			}
			val  = node->val_;//如果等于sentienl_ 给填充个默认值
			return node !=sentienl_;
		}
		//模仿std::map实现一个 即可当右值又可以做左值的功能,做右值求值,做左值赋值
		Value& operator[](const Key&key )
		{
			Node* node = root_;
			while( node!=sentienl_ && node->key_!=key )
				node = node->key_>key?node->left_:node->right_;
			if( node !=sentienl_ )
				return node->val_;
			//插入一个空值,
			insert(key,Value());
			//todo 略low~~~ 待优化
			return this->operator[](key);
		}
		bool IsBanlace()const//检查红黑树是否符合要求
		{
			return NodeIsBlack(root_) && IsBanlace(root_);
		}
		size_t BlackHeight()const
		{
			return blackheight(root_);
		}
		//插入  难度- 中等
		bool insert(const Key&key, const Value& val )override
		{
			if( root_ == sentienl_ )//空树
			{
				root_ = makeNewNode(key,val,nullptr);
				SetNodeBlack(root_);
				++size_;
				return true;
			}
			Node* node=root_,*parent = nullptr,**temp;
			while( node!=sentienl_ && node->key_ != key )
			{
				parent = node;
				if( key>node->key_ )
					temp = &node->right_;
				else
					temp = &node->left_;
				node = *temp;
			}
			if( node != sentienl_ )//已经存在 修改
			{
				node->val_ = val;
				return false;
			}
			//不存在插入 默认是红色,子节点是sentienl_
			node = makeNewNode(key,val,parent);
			*temp = node;
			//调整
			reBalance_Insert(node);
			return true;
		}
		bool deletenode(const Key&key )override
		{
			//删除比较难~~~~~~
			Node* node = root_;
			while( node!=sentienl_ && node->key_!=key)
				node = node->key_>key?node->left_:node->right_;
			if( node == sentienl_ )//没找到
				return false;
			
			//temp要替代的节点 subst要删除的节点,temp是要替代原节点进行红黑树调整的
			Node* temp,*subst;

			//存在两个子节点的时候,用右子树的最小(node的后继,替换node )
			//存在一个子节点的时候用另一个孩子替换
			if( node->right_!=sentienl_ && node->right_!=sentienl_ )
			{
				subst = node->right_;
				while(subst->left_!=sentienl_ )
					subst = subst->left_; 
				//此时的subst必然最多还有右孩子,不可能有左孩子,因为subst是node->right_树里最小的
				temp = subst->right_;
			}
			else
			{
				subst = node;
				temp = node->right_!=sentienl_?node->right_:node->left_;
			}
			
			if( subst == root_ )//这种情况只有树是个单边的树,那么可以直接把根节点去掉换成唯一的左孩子或者右孩子
			{
				root_ = temp;
				delete subst;
				size_--;
				SetNodeBlack(root_);
				root_->parent_=nullptr;
				return true;
			}
			//修改原来父节点的指针域 此时subst不是根节点必然存在父节点
			if( subst == subst->parent_->left_ )
				subst->parent_->left_ = temp;
			else
				subst->parent_->right_= temp;

			//替换子节点的父节点指针域
			bool isRed = NodeIsRed(subst);//这个subst指针后面会改所以这里提前存一下
			if( subst == node )//这是没有替换的情况
			{
				temp->parent_ = subst->parent_;
			}
			else
			{
				//这里是吧node的内容替换成subst node的颜色不变,然后把subst删掉
				node->key_ = subst->key_;
				node->val_ = subst->val_;
				temp->parent_ = subst->parent_;
		
				if( subst->parent_ == sentienl_ )//删除的是根节点
					root_ = temp;
				else
				{
					if( subst == subst->parent_->left_ )
						subst->parent_->left_ = temp;
					else
						subst->parent_->right_ = temp;
				}
				//原来的node节点相当于没动 所以node的左右孩子也不需要动 删除subst
				node = subst;			
			}
			delete node;
			size_--;
			//重新调整红黑树
			if(!isRed)//删除红色节点不用调整跳出
				reBalance_delete(temp);	
			return true;
		}
	private:
		void reBalance_delete(Node* node )
		{
			Node*  bro;//兄弟节点
			/*	
				删除的是黑色节点,即待调整节点带了一个黑色属性,当待调整节点为红色时,可以直接把它调整成黑色,结束
			*/
			while( node!=root_&&NodeIsBlack(node) )
			{
				if(node == node->parent_->left_ )//当前节点时左孩子
				{
					bro = node->parent_->right_;
					/*
						case1: 兄弟节点是红色,父节点,兄弟节点的孩子节点,都是黑色,此时需要转化成后面的情况处理,
								1:兄弟节点变黑
								2:父节点变红
								3:父节点左旋
								4:重新设置兄弟节点为原兄弟节点的左孩子
					*/ 
					if( NodeIsRed(bro) )
					{
						SetNodeBlack(bro);
						SetNodeRed(node->parent_);
						leftRotate(node->parent_);
						bro = node->parent_->right_;
					}

					/*
					 	case2: 兄弟节点是黑色,两个孩子都是黑色,此时父节点看左子树少了一个黑节点,所以只需要让右结点也少一个黑节点即可
							1:兄弟节点变成红色
							2:父节点变成新的调整节点
							此时相当于调整节点向上转移了
					*/
					if( NodeIsBlack(bro->left_) && NodeIsBlack(bro->right_) )
					{
						SetNodeRed(bro);
						node = node->parent_;
						//进行下一次调整
						continue;
					}
					else
					{
						/*
							case3: 兄弟节点是黑色,左孩子是红色此时需要转化成case4的情况
							1:左孩子变黑色
							2:兄弟节点变红色
							3:兄弟节点右旋
							4:重新设置兄弟节点
						*/

						if( NodeIsBlack(bro->right_) )//这里判断一定写右孩子为黑,因为有可能左右都是红,这种统一在下个case处理
						{
							SetNodeBlack(bro->left_);//左孩子必然是红,因为两个都是黑前面判断了
							SetNodeRed(bro);
							rightRotate(bro);
							bro = node->parent_->right_;
						}
						/*
							case4: 兄弟节点是黑色,右孩子是红色,左孩子是任意色
							1:父节点的颜色拷贝给兄弟节点
							2:父节点设置为黑色
							3:兄弟节点的右孩子设置为黑色
							4:父节点左旋 
							break 跳出循环
							这里parent(新的parent)颜色保持不变,左子树多了一个黑节点,右子树的右结点变成了黑色,黑色节点没变

						*/
						bro->color_ = node->parent_->color_;
						SetNodeBlack(node->parent_);
						SetNodeBlack(node->right_);
						rightRotate(node->parent_);
						break;
					}
				}
				else//node结点是右孩子的情况类似
				{
					bro = node->parent_->left_;
					if( NodeIsRed(bro) )
					{
						/*
							兄弟节点为红色
							1:兄弟节点变黑
							2:父节点变红
							3:父节点右旋
							4:重新设置兄弟节点
						*/
						SetNodeBlack(bro);
						SetNodeRed(node->parent_);
						rightRotate(node->parent_);
						bro = node->parent_->left_;
					}

					if( NodeIsBlack(bro->left_) && NodeIsBlack(bro->right_) )
					{
						/*
							兄弟节点的两个孩子都是黑色
							1:设置兄弟节点为红色
							2:转移调整节点为父节点
						*/
						SetNodeBlack(bro);
						node = node->parent_;
						continue;
					}
					else
					{
							/*
							兄弟节点的右孩子是红色,左孩子是黑,这里一定是以左孩子为黑作为依据,左右都是红的留在下个case处理
							1:右孩子设置成黑
							2:兄弟节点设置为红
							3:兄弟节点左旋
							4:重新设置兄弟节点
							为case4 准备
						*/
						if( NodeIsBlack(bro->left_) )//这里一定是以右孩子为黑为判断依据,两个都红留在后面处理
						{
							SetNodeBlack(bro->right_);//这个一定是黑
							SetNodeRed(bro);
							leftRotate(bro);
							bro = node->parent_->left_;
						}
						/*
							兄弟节点为黑,兄弟节点的左孩子为红,兄弟节点的右孩子任意
							1:把父节点的颜色拷贝给兄弟节点
							2:父节点变黑
							3:左孩子变黑
						    4:父节点右旋
							结束
						*/
						bro->color_ = node->parent_->color_;
						SetNodeBlack(node->parent_);
						SetNodeBlack(bro->left_);
						rightRotate(node->parent_);
						break;
					}
				}
			}
			SetNodeBlack(node);
		}
		void reBalance_Insert(Node* node )
		{
			//调整的结束条件是 父节点是黑色,或者本身是跟节点
			Node* gp;//gp 祖父节点 uncle 叔叔节点
			while( node != root_ && NodeIsRed(node->parent_) )
			{
				//父节点是红色,则一定存在祖父节点(红黑树根节点是黑色)
				// 叔叔节点如果是红色的,只需要将叔叔节点和父节点变成黑色,祖父节点变成红色,这样红黑树的黑高是不变的,然后把祖父节点变成node
				// 再往上调整
				// 叔叔节点如果是黑色,这时需要根据node节点和他的父节点,及他的父节点和祖父节点的位置来不同的处理
				// 左左 node节点是它父节点的左孩子,父节点是祖父节点左孩子,祖父节点右旋,然后把祖父节点变成红色,父节点变成黑色 这样黑高不变,
				//且红黑树不出现连续的红色节点
				//左右 node节点是他的父节点右孩子,父节点是祖父节点的左孩子,先把父节点左旋转,然后变成上面的情况了
				// 右右 node节点是它父节点的右孩子,父节点是祖父节点的右孩子,祖父节点左旋,祖父节点变成红色,父节点变成黑色,然后继续以父节点往上调整
				// 右左 node节点是它的父节点左孩子,父节点是祖父节点的右孩子,先把父节点右旋,然后再把祖父节点左旋,祖父节点变红,父节点变黑
				// 除了变色操作其他和AVL树一毛一样
				// 变色调整会迭代往上操作,别的只调整一次就不用动了
				//父节点是红色 则必然存在祖父节点
				gp = node->parent_->parent_;
				//父节点是祖父节点的左孩子
				if( node->parent_ == gp->left_ )
				{
					//叔叔节点是红色,变色,会往上递归  节点不存在为sentienl节点 为黑色 和节点为黑可以统一
					if( NodeIsRed(gp->right_) )
					{
						//叔叔节点/父节点变黑色 祖父节点变红色,祖父节点作为新的待调整节点继续往上
						SetNodeBlack(node->parent_);
						SetNodeBlack(gp->right_);
						SetNodeRed(gp);
						node = gp;
						continue;
					}
					//叔叔节点是黑色,分情况 自己是右孩子则父节点左旋,然后变成左左情况	
					if( node == node->parent_->right_ )//左右情况
					{
						node = node->parent_;
						//左左情况 node节点是最下面的节点,这时经过旋转父节点变成最下面的节点
						leftRotate(node);	
					}
					//统一变成左左情况情况,祖父节点变红,父节点变黑,右旋
					SetNodeRed(gp);
					SetNodeBlack(node->parent_);
					rightRotate(gp);
					
				}
				else//父节点是祖父节点的右孩子
				{
					//叔叔节点是红色
					if( NodeIsRed(gp->left_) )
					{
						//变色 父节点 叔叔节点变黑 祖父节点变红
						SetNodeBlack(node->parent_);
						SetNodeBlack(gp->left_);
						SetNodeRed(gp);
						node = gp;
						continue;//左右子树黑高+1 当整个树没变
					}
					//叔叔节点是黑色
					if( node == node->parent_->left_ )//右左情况 通过旋转变成右右
					{
						node = node->parent_;
						rightRotate(node);
					}
					//右右 祖父节点变红 父节点变黑 祖父节点左旋
					SetNodeRed(gp);
					SetNodeBlack(node->parent_);
					leftRotate(gp);
				}
			} 
			//最终可能出现 跟节点变红,直接根节点变黑 这次整个树的黑高增加了,这也是唯一会让整个树黑高增加的地方
			SetNodeBlack(root_);
		}
		//左旋
		/*
			本质上跟AVL树一毛一样
			但是这里增加了每个节点的父节点属性
			左旋 右孩子的左子树赋值给父节点的右子树,父节点作为右孩子的左子树
		*/
		void leftRotate(Node* node )
		{
			Node* right = node->right_;
			node->right_ = right->left_;
			right->left_ = node;
			//修改右孩子的左孩子的父指针域
			if( node->right_!=sentienl_ )
				node->right_->parent_ = node;

			if( node->parent_ == nullptr )//跟节点
			{
				root_ = right;
			}
			else
			{
				//修改父节点对应的指针域
				if( node == node->parent_->left_ )
					node->parent_->left_ = right;
				else
					node->parent_->right_ = right;
			}
			//修改当前节点中的parent指针  和右孩子中的parent指针域
			right->parent_ = node->parent_;
			node->parent_ = right;
		}
		/*右旋*/
		void rightRotate(Node* node )
		{
			Node* leftchild = node->left_;
			//自己作为左孩子的右孩子 左孩子的右孩子作为自己的左孩子
			node->left_ = leftchild->right_;
			leftchild->right_ = node;
			//修改原左孩子的右孩子的父亲指针
			if( node->left_ !=sentienl_ )
				node->left_->parent_ = node;
			//修改node节点的父节点中的指针域
			if( node->parent_ == nullptr )
				root_ = leftchild;
			else if( node == node->parent_->left_ )
				node->parent_->left_ = leftchild;
			else
				node->parent_->right_ = leftchild;
			//修改node节点和他的左孩子节点中的父指针域
			leftchild->parent_ = node->parent_;
			node->parent_ = leftchild;
		}
		bool IsBanlace(Node* node )const
		{
			//检查 根节点为黑(第一步就检查了)  当前节点为红则父节点 子节点都不会是红色
			// 各个子树的黑高相等
			if( node == sentienl_ )
				return true;
			//没有连续的两个红色节点
			if( NodeIsRed(node) )
			{
				if( NodeIsRed(node->parent_) )
					return false;
				if( NodeIsRed(node->left_) || NodeIsBlack(node->right_) )
					return false;
			}
			return blackheight(node->left_) == blackheight(node->right_);
		}
		size_t blackheight(Node* node )const
		{
			if( node == sentienl_ )
				return 1;
			return std::max<size_t>( blackheight(node->left_), blackheight(node->right_) )+NodeIsBlack(node)?1:0;
		}
		void destruct(Node*node )
		{
			if(node!=sentienl_)
			{
				destruct(node->left_);
				destruct(node->right_);
				delete node;
			}
		}
		//新建节点默认是红色 子节点为sentienl_ 
		Node* makeNewNode( const Key& key, const Value& val,Node* parent )
		{
			Node* node = new Node( key,val,parent );
			node->left_ = sentienl_;
			node->right_ = sentienl_;
			SetNodeRed(node);
			return node;
		}
		size_t height(Node* node )const
		{
			if( node == sentienl_ )
				return 0;
			return std::max<size_t>( height(node->left_), height(node->right_))+1;
		}
		void _preorder(const DealKey& deal , Node* node )const
		{
			if(node !=sentienl_ )
			{
				deal(node->key_);
				_preorder(deal,node->left_);
				_preorder(deal,node->right_);
			}
		}
		void _inorder(const DealKey& deal, Node* node )const
		{
			if( node!= sentienl_ )
			{
				_inorder(deal,node->left_);
				deal(node->key_);
				_inorder(deal,node->right_);
			}
		}
		void _postorder(const DealKey& deal, Node* node )const
		{
			if(node!=sentienl_ )
			{
				_postorder(deal,node->left_);
				_postorder(deal,node->right_);
				deal(node->key_);
			}
		}
		Node*     root_;
		size_t    size_;
		Node*     sentienl_;//哨兵节点，根节点的父节点，叶节点子孩子都指向这个值
	};

}//namespace
#endif