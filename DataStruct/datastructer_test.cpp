#include<iostream>
#include<functional>
#include<string>
#include<memory>
#include<map>
#include<random>
#include "test.hpp"
#include<ctime>
#include<cstdlib>
#include<chrono>
using namespace std;
using namespace datastruct;
void bracket_match();//括号匹配
#define TestSize 59999
#define PrintArray(arry) std::for_each(arry,arry+sizeof(arry)/sizeof(arry[0]), [](decltype(arry[0]) t){std::cout<<t<<" ";});
template<typename T>
void randomshuffle(T& arry)
{
	std::random_device rd;
	std::mt19937 engine(rd());
	std::shuffle(arry.begin(), arry.end(), engine);
}
struct TimeCounts {
private:
	uint64_t   counts_;
	std::string str_;
public:
	explicit TimeCounts(const std::string& msg) :str_(msg)
	{
		counts_ = now();
	}
	uint64_t now()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	}
	~TimeCounts()
	{
		std::cerr << str_ << " 耗时: " << now() - counts_ << " ms " << std::endl;
	}
};
struct SelfDefine{
    public:
    int number_=0;
    std::string str_;
    public:
    ~SelfDefine()
    {
    }
    SelfDefine(){};
    SelfDefine( int a, const std::string& str ):number_(a),str_(str){};
    friend std::ostream& operator<<(std::ostream& os, const SelfDefine& t );
    bool operator == (const SelfDefine& oth )
    {
        return number_ == oth.number_ && str_ == oth.str_ ;
    }
	bool operator >(const SelfDefine& oth)
	{
		if (number_ != oth.number_)
			return number_ > oth.number_;
		else
			return str_ > oth.str_;
	}
	bool operator<(const SelfDefine& oth)
	{
		if (number_ != oth.number_)
			return number_ < oth.number_;
		else
			return str_ < oth.str_;
	}
};
int Random()
{
    static bool init=false;
    if( !init )
    {
#ifdef __linux__
        srandom(time(NULL));
#else
		srand(time(NULL));
#endif
        init=true;
    }
#ifdef __linux__
    return 1000+random()%1000;
#else
	return 1000 + rand() % 1000;
#endif
}
std::ostream& operator<<(std::ostream&os, const SelfDefine&t )
{
    os<<"number:"<<t.number_<<" str:"<<t.str_<<std::endl;
    return os;
}
void stack_test( int argc, char**argv  )
{
    if( argc<2)
       return;
    int type = atoi(argv[1]);
    int count =0;
    if( argc>2 )
        count  = atoi(argv[2]);  
    StackBase<int>* ptr = nullptr;
    if( type == 1 )
        ptr = new ArrayStack<int>;
    else
        ptr = new LinkedListStack<int>;
    assert( ptr );
    std::shared_ptr<int> autofree(NULL, [ptr](int*t){ delete ptr;});
    int a[]={1,900,200,300,998};

    if( count <= 0 )
        std::for_each(a, a+sizeof(a)/sizeof(int), [ptr](int v ){ ptr->Push(v);});
    else 
    {
        for( int i=0;i<count;i++ )
            ptr->Push(i);
    }

    std::cout<<(type==1?"ArrayStack test: ":"LinkedListStack test: ")<<std::endl;
    std::cout<<ptr->Size()<<std::endl;
    while( !ptr->IsEmpty() )
    {
        std::cout<<ptr->Top()<<" ";
        ptr->Pop();
    }
    std::cout<<std::endl;

    StackBase<SelfDefine>* tt = nullptr;
    if( type == 1 )
        tt = new ArrayStack<SelfDefine>;
    else
        tt = new LinkedListStack<SelfDefine>;
    std::shared_ptr<int> autofree2(NULL, [tt](int*t){ delete tt;});

    tt->Push( SelfDefine(10,"1" ));
    tt->Push( SelfDefine(11,"2" ));
    tt->Push( SelfDefine(12,"3" ));
    tt->Push( SelfDefine(13,"4" ));

    while( !tt->IsEmpty() )
    {
        std::cout<<tt->Top();
        tt->Pop();
    }
    std::cout<<std::endl;
    std::cout<<"test end "<<std::endl;

    //迭代器测试
    std::shared_ptr<ArrayStack<SelfDefine>> ttt = std::make_shared<ArrayStack<SelfDefine>>();
    for( int i=0;i<40;i++ )
    {
        ttt->Push(SelfDefine(i,"这是第 "+std::to_string(i)+" 个" ));
    }
    for( ArrayStack<SelfDefine>::iterator it = ttt->begin();it!=ttt->end();++it )
        cout<<*it<<endl;
    //bracket_match();
}
//括号匹配问题
void bracket_match()
{
    std::string str;
    static std::map<char,char> matchs={
        {']','['},
        {'}','{'},
        {')','('}
        };
    while( std::cin>>str )
    {
        LinkedListStack<char> st;
        for( int i=0;i<str.size();i++ )
        {
            switch(str[i])
            {
                case '[':
                case '(':
                case '{':
                st.Push(str[i]);
                break;
                case ']':
                case ')':
                case '}':
                if( matchs[str[i]] == st.Top() )
                    st.Pop();
                else
                    std::cout<<"not match"<<endl;
                break;
                default:
                    std::cout<<"not match"<<endl;
                    break;
            }
        }
        if( !st.IsEmpty())
            std::cout<<"not match"<<endl;
        else
            std::cout<<"match"<<endl;
    }
}

void priority_queue_test(int argc, char**argv )
{
    int count =0;
    if( argc>1 )
        count = atoi(argv[1]);

    std::shared_ptr<PriorityQueueBase<int>> ptr;

    int a[]={10,90,2,1,100,1000,456,-2,9000,456};
    if( count == 0 )
    {
        ptr = std::make_shared<ArrayPriorityQueue<int>>( a, sizeof(a)/sizeof(int));
        PrintArray(a); 
        cout<<endl;
    }
    else
    {
        ptr = std::make_shared<ArrayPriorityQueue<int>>();
        for( int i=0;i<count;i++ )
            ptr->push(Random());
    }

    cout<<"ArrayPriorityQueue test: "<<endl;
    std::cout<<std::endl;
    while(!ptr->empty())
    {
        cout<<ptr->top()<<" ";
        ptr->pop();
    }
    std::cout<<endl;
    cout<<__FUNCTION__<<"test end"<<endl;

}
void queue_test( int argc,char**argv )
{
    int type=0;
    if( argc>1 )
        type = atoi(argv[1]);
    std::shared_ptr<QueueBase<std::string>> ptr;
    
    if( type == 0 )
        ptr = std::make_shared<ArrayQueue<std::string>>();
    else
        ptr = std::make_shared<LinkedListQueue<std::string>>();

    ptr->push("aaa");
    assert( ptr->back() == ptr->front() );
    ptr->push("bbb");

    assert( ptr->size() ==2 );

    assert( ptr->back() == "bbb" );
    assert( ptr->front() == "aaa" );

    ptr->pop();
    cout<<ptr->front()<<endl;
    cout<<ptr->back()<<endl;

    std::shared_ptr<QueueBase<SelfDefine>> selfptr;
    if( type == 0 )
        selfptr = std::make_shared<ArrayQueue<SelfDefine>>();
    else
        selfptr = std::make_shared<LinkedListQueue<SelfDefine>>();

    for( int i=0;i<TestSize;i++ )
        selfptr->push( SelfDefine(i, "aaaa" ));

    assert( selfptr->size() ==TestSize );
    assert( selfptr->back().number_ == TestSize-1 );
    assert( selfptr->front().number_ == 0 );
    selfptr->pop();
    assert( selfptr->back().number_ == TestSize-1 );
    assert( selfptr->front().number_ == 1 );

    if( type ==0 )
    {
        cout<<dynamic_cast<ArrayQueue<std::string>*>(ptr.get())->capacity()<<endl;
        cout<<dynamic_cast<ArrayQueue<SelfDefine>*>(selfptr.get())->capacity()<<endl;
    }

    while( ptr->size() || !ptr->empty() )
        ptr->pop();
    while( selfptr->size()||!selfptr->empty())
        selfptr->pop();
    assert( ptr->empty() && ptr->size()==0);
    assert( selfptr->empty() && selfptr->size()==0);
    cout<<__FUNCTION__<<" test end"<<endl;

}
void deque_test(int argc,char**argv )
{
    int type=0;
    if( argc>1 )
        type =1;

    DequeBase<std::string>*  ptr;
    DequeBase<SelfDefine>*   selfptr;
    if( type == 0 )
    {
        ptr = new ArrayDeque<std::string>();
        selfptr = new ArrayDeque<SelfDefine>();
    }
    else 
    {
        ptr = new LinkedListDeque<std::string>();
        selfptr = new LinkedListDeque<SelfDefine>();
    }

    ptr->push_back("abc");

    assert( ptr->front() == ptr->back());

    ptr->pop_front();
    assert( ptr->size()==0 );

    ptr->push_front("aaa");
    assert( ptr->front() == ptr->back());

    ptr->push_back("abc");

    assert(ptr->size()==2 && ptr->back() == "abc" && ptr->front() == "aaa" );

    ptr->pop_back();
    ptr->push_front("eee");
    assert( ptr->back() == "aaa" );

    std::string temp = ptr->front();
    for( int i=0;i<TestSize;i++ )
        ptr->push_back( std::to_string(i) );

    assert( ptr->front() == temp );
    assert( ptr->back() == std::to_string( TestSize-1 ) );

    while( !ptr->empty() )
    {
        ptr->pop_back();
        if( !ptr->empty() )
            ptr->pop_front();
    }

    for( int i=0;i<TestSize;i++ )
        selfptr->push_front( SelfDefine( i, std::to_string(i) ) );

    cout<<selfptr->front()<<endl;
    cout<<selfptr->back()<<endl;
    assert( selfptr->back() == SelfDefine( 0,"0" ) && selfptr->front() == SelfDefine( TestSize-1,std::to_string(TestSize-1) ) );

    assert( selfptr->size() == TestSize );

    while( !selfptr->empty())
    {
        selfptr->pop_front();
        if( !selfptr->empty() )
            selfptr->pop_back();
    }
    assert( selfptr->size()==0 && selfptr->empty() );


    cout<<__FUNCTION__<<" test end"<<endl;
    delete ptr;
    delete selfptr;
}


void binarySearchTree_test(int argc, char**argv)
{
	BaseBinarySearchTree<int, std::string> *tree;
	BaseBinarySearchTree<int, SelfDefine>*self;
	if (argc == 1)
	{
		//BST树
		tree = new BinarySearchTree<int, std::string>();
		self = new BinarySearchTree<int, SelfDefine>();
	}
	assert(tree != nullptr && self != nullptr);
	int a[] = { 10,1,19,20,23,22,24,40,11,2,3,12,14,17,7,4,5,6 };
	/*
	前序遍历
	10  1  2  3  7  4  5  6  19  11  12  14  17  20  23  22  24  40
	中序遍历
	1  2  3  4  5  6  7  10  11  12  14  17  19  20  22  23  24  40
	后序遍历
	6  5  4  7  3  2  1  17  14  12  11  22  40  24  23  20  19  10
	*/
	for (int i = 0; i < sizeof(a) / sizeof(int); i++)
	{
		tree->insert(a[i], std::to_string(a[i]));
	}
	{
		//查找
		std::string str = tree->findmax();
		assert(tree->findmax() == "40");
		assert(tree->findmin() == "1");
		std::string ret;
		assert(tree->find(19, ret) && ret== "19");
		assert(tree->find(11, ret) && ret == "11");
		assert(!tree->find(101, ret));
	}
	
	{
		//前驱后继
		std::string ret;
		assert(tree->getprenode(20, ret));
		assert(ret == "19");
		assert(tree->getprenode(100, ret) == false);
		tree->getnextnode(20, ret);
		assert(ret == "22");//后继
		assert(!tree->getnextnode(40,ret));//40没有后继
		//assert(!tree.getprenode(1, ret));//1没有前驱
	}
	auto Print = [](int a) {
		std::cout << a << " ";
	};
	std::cout << "前序遍历" << endl;
	tree->preorder(Print);
	std::cout << endl;
	
	std::cout << "非递归的前序遍历" << endl;
	tree->no_recursive_preorder(Print);
	std::cout << endl;

	std::cout << "中序遍历" << endl;
	tree->inorder(Print);
	std::cout << endl;

	std::cout << "非递归的中序遍历" << endl;
	tree->no_recursive_inorder(Print);
	std::cout << endl;

	std::cout << "后序遍历" << endl;
	tree->postorder(Print);
	std::cout << endl;

	std::cout << "非递归的后序遍历" << endl;
	tree->no_recursive_postorder(Print);
	std::cout << endl;

	{
		//删除
		std::cout << "原来的中序遍历" << endl;
		tree->inorder(Print);
		std::cout << endl;
		assert(!tree->deletenode(100));

		auto deletefunction = [&](int key) {
			assert(tree->deletenode(key));
			std::cout << "删除 "<<key<<" 之后的中序遍历" << endl;
			tree->inorder(Print);
			std::cout << endl;
		};
		for (int i = 0; i < sizeof(a) / sizeof(int); i++)
		{
			deletefunction(a[i]);
		}

	}
	{
		
		std::vector<int> numbers;
		for (int i = 0; i < TestSize; i++)
		{
			numbers.push_back(i);
		}
		//打乱数组 按顺序插入会让bst退化成链表
		randomshuffle(numbers);
		{
			TimeCounts t("插入 " + std::to_string(TestSize) + " 条数据 ");
			for (int i = 0; i < numbers.size(); i++)
			{
				std::string str = "这是第 " + std::to_string(numbers[i]) + " 个测试对象 ";
				self->insert(numbers[i], SelfDefine(numbers[i], str));
			}
		}

		assert(self->size() == TestSize);
		/*
		1: 测试删除
		2: 测试查找
		3: 中序遍历结果打印
		4: 前驱结点
		5: 后继结点

		*/
		auto searchfunction = [&self]() {
			int key = -1;
			std::cout << "输入key" << std::endl;
			std::cin >> key;
			SelfDefine ret;
			bool isOk = false;
			{
				TimeCounts t("查找 " + std::to_string(key));
				isOk = self->find(key, ret);
			}
			if (isOk)
				cout << " Find " << ret << endl;
			else
				cout << " NOT Find " << endl;	
		};
		auto delfunction = [&self]() {
			int key = -1;
			std::cout << "输入key" << std::endl;
			std::cin >> key;
			bool isOk = false;
			{
				TimeCounts t("删除 " + std::to_string(key));
				isOk = self->deletenode(key);
			}
			if (isOk)
				cout << " 删除成功 "<< endl;
			else
				cout << " NOT Find " << endl;
		};
		auto inorderfunction = [&self]() {
			std::vector<int> ret;
			ret.reserve(self->size());
			auto savekey = [&ret](int a) { 
				ret.push_back(a);
			};
			{
				TimeCounts t("中序遍历");
				self->inorder(savekey);
			}
			std::for_each(ret.begin(), ret.end(), [](int a) {std::cout << a << " "; });
		};
		auto  findpreornext = [&self](int opt) {
			SelfDefine ret;
			int key = -1;
			std::cout << "输入key" << std::endl;
			std::cin >> key;
			std::string msg = (opt == 1 ? " 前驱结点 " : "后继结点");
			bool isOk = false;
			{
				TimeCounts t("查找 "+msg );
				if (opt == 1)
					isOk = self->getprenode(key, ret);
				else
					isOk = self->getnextnode(key, ret);
			}
			if (isOk)
				cerr << " 找到 " << msg << " " << ret << endl;
			else
				cerr << " 没找到 " << msg << endl;
		};
		auto insertfunction = [&self]() {
			int key = -1;
			std::cout << "输入key" << std::endl;
			std::cin >> key;
			bool isok = false;
			{
				TimeCounts t("插入");
				isok = self->insert(key, SelfDefine(key, "这是第 " + std::to_string(key) + " 个测试对象 "));
			}
			if (isok)
				cerr << "插入成功 " << endl;
			else
				cerr << " 插入重复 " << endl;
		};
		auto findMaxOrMin = [&self](int opt) {
			std::string str = opt == 1 ? " 最大 " : " 最小 ";
			SelfDefine ret;
			{
				TimeCounts t("查找 " + str);
				if (opt == 1)
					ret = self->findmax();
				else
					ret = self->findmin();
			}
			std::cout << str << ret << endl;
		};
		bool end = false;
		while (!end)
		{
			int number = 0;
			std::cerr << " \t1:删除\n\t2:查找\n\t3:中序遍历输出结果\n"
				"\t4:前驱结点\n\t5:后继结点\n\t6:插入\n\t7:最大结点\n\t8:最小\n\t9:bst size\n\t-1:退出\n";
			std::cin >> number;
			SelfDefine ret;
			switch (number)
			{
			case 1:
				delfunction();
				break;
			case 2:
				searchfunction();
				break;
			case 3:
				inorderfunction();
				break;
			case 4:
				findpreornext(1);
				break;
			case 5:
				findpreornext(0);
				break;
			case 6:
				insertfunction();
				break;
			case 7:
				findMaxOrMin(1);
				break;
			case 8:
				findMaxOrMin(0);
				break;
			case 9:
				std::cout << "Size: " << self->size() << endl;
				break;
			case -1:
				std::cin.clear();
				end = true;
				break;
			default:
				std::cout << "输入错误" << endl;
				break;
			}
		}
	}

	cout << __FUNCTION__ << " test end" << endl;
	delete tree;
	delete self;
}













