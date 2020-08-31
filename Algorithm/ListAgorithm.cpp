/*
 *  输入一个链表，输出该链表中倒数第k个节点。为了符合大多数人的习惯，本题从1开始计数，即链表的尾节点是倒数第1个节点。例如，一个链表有6个节点，从头节点开始，它们的值依次是1、2、3、4、5、6。这个链表的倒数第3个节点是值为4的节点
 *      例如:
 *      给定一个链表: 1->2->3->4->5, 和 k = 2.
 *      返回链表 4->5.
 *      思路:
 *          快慢指针,快指针先走k步,然后快慢指针一起走,每次走一步,快指针到头的时候 慢指针所在的位置就是倒数第几个元素
struct ListNode {
    int val;
    struct ListNode *next;
};
/*
	还有一个类似的题
	给定一个链表，删除链表的倒数第 n 个节点，并且返回链表的头结点。

	示例：

	给定一个链表: 1->2->3->4->5, 和 n = 2.

	当删除了倒数第二个节点后，链表变为 1->2->3->5.

*/
#include<stdlib.h>
#include<stdio.h>
#include "BaseAlgorithm.hpp"
class ListGetKthFromEnd:public AlgorithmSolution
{
    public:
    ListGetKthFromEnd()
    {
        m_algorithmName="链表查找倒数第K个值 删除倒数第K个值 双指针法";
    }
    void Solution()override
    {
        List<int> l={1,2,3,4,5,6,7,8};
        std::cout<<"原始链表"<<std::endl;
        l.print();
        int k=4;
        auto node = getKthFromEnd(l.head(),k);
        std::cout<<"倒数第 "<<k<<"个值为: "<<node->val<<std::endl;
        removeFromEnd(l,k);
        std::cout<<"删除倒数第 "<<k<<" 个值后的链表为: "<<std::endl;
        l.print();
    }
    template<typename T>
    ListNode<T>* getKthFromEnd(ListNode<T>* head, int k )
    {
        if( head == nullptr || k<=0 )
            return nullptr;
        ListNode<T>* fast = head,*slow = head;
        int i=0;
        for( ;i<k && fast!=nullptr ;i++)
            fast = fast->next;
        if( fast == nullptr && i == k )
            return slow;
        if( fast == nullptr )
            return nullptr;
        while( fast!=nullptr )
        {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
    template<typename T>
    void removeFromEnd(List<T>&l, int k )
    {
        auto head = l.head();
        decltype(head) fast=head, slow=head;
        int i=0;
        for(;i<k&&fast!=nullptr;++i )
            fast = fast->next;
        
        do
        {
            if( fast == nullptr && i!=k )
                return ;
            while( fast!=nullptr )
            {
                fast = fast->next;
                slow = slow->next;   
            }
            l.erase(slow);
        }while(0);

    }
};
RegistAlgorithm(ListGetKthFromEnd)
/*
 * 判断链表是否有环并且找到环所在的位置
 * 判断是否有环 可以通过快慢指针,快指针每次走2步  慢指针每次走1步
 * 只要有环 进入环后,快指针速度比慢指针快,那么快指针最终会追上慢指针的
 * 当追上的时候 此时慢指针总共走了 N步的话 那么快指针就是2N步
 * 假设他们相遇在M点,那么此时把快指针再从头开始走,一次一步,满指针也继续从M点走一次也一步
 * 因为他俩速度一样, 那么他俩走N步必然会再次在M点集合,此时 他们第一次相遇的点 就是环的位置
 * */
class FindListCircle:public AlgorithmSolution
{
    public:
    FindListCircle()
    {
        m_algorithmName="查找链表是否有环，并找到环的位置";
    }
    void Solution()override
    {
        AlgorithmSolution::Solution();
        List<int> l={1,2,3,4,5,6,7,8};

        auto node = l.head();
        node = node->next->next;

        auto temp = l.head();
        while( temp->next !=nullptr )
            temp = temp->next;
        
        //造一个环 ，让8的next指针指向3
        temp->next = node;

        auto test=[this](ListNode<int>*head){
            auto temp = findListCircle(head);
            if( temp )
            {
                std::cout<<"链表有环，环的节点的值为 "<<temp->val<<std::endl;
            }
            else
                std::cout<<"链表没环"<<std::endl;
            return;
        };
        
        decltype(l) l2={1,2,3,4,5,6,7,8,10,100};

        test(l.head());
        test(l2.head());
    }
    private:
    template<typename T>
    ListNode<T>* findListCircle(ListNode<T>*head )
    {
        decltype(head) fast = head,slow=head,node;
        //先判断是否有环，快慢指针，快指针走两步，慢指针走一步
        bool bhaveCircle=false;
        while( slow && fast && fast->next )
        {
            if( slow == fast && slow !=head )
            {
                bhaveCircle = true;
                break;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        if( !bhaveCircle )
            return nullptr ;//无环
        //确定有环后，把fast指针挪到头上，这是快慢指针都每次一个步长
        //因为上面相遇的时候，快指针走的路程是慢指针的2蓓，那么快指针从头开始走到这里，满指针再走这么多步
        //两个必然相遇，第一次相遇的点就是环的位置
        fast = head;
        decltype(slow) parent;
        while( fast && slow && slow != fast )
        {
            parent = slow;
            fast = fast->next;
            slow = slow->next;
        }
        //这里多加一步，不然这个有环的链表就析构不了了，在环里的slow指针上一次指向的就是这个环
        //让它指向nullptr保证析构掉
        parent->next = nullptr;
        return fast;
    }
};
RegistAlgorithm(FindListCircle)
/*
输入两个递增排序的链表，合并这两个链表并使新链表中的节点仍然是递增排序的。
*/
class MergeOrderedList:public AlgorithmSolution
{
    public:

    MergeOrderedList()
    {
        m_algorithmName = "合并有序链表";
    }
    void Solution()override
    {
        List<int>l1;
        List<int>l2;
        for( int i=20;i>=0;i-- )
        {
            if( i & 1)
                l1.insert(i);
            else
                l2.insert(i);
        }
        std::cout<<"原始链表l1: "<<std::endl;
        l1.print();
        std::cout<<"原始链表l2:"<<std::endl;
        l2.print();
        List<int>l3(mergeTwoLists2(l1.head(),l2.head()));

        std::cout<<"合并后的链表"<<std::endl;
        l3.print();
    }
    private:
    //二级指针做法 更优雅
    template<typename T>
    ListNode<T>* mergeTwoLists(ListNode<T>*l1,ListNode<T>*l2)
    {
        if(!l1 || !l2 )
            return l1?l1:l2;
        ListNode<T>*head,**node = &head;//引用不能被重新赋值所以这里不许是二级指针
        while( l1!=nullptr && l2!=nullptr )
        {
            if( l1->val<l2->val )
            {
                *node = l1;
                l1 = l1->next;
            }
            else
            {
                *node = l2;
                l2 = l2->next;
            }
            node = &(*node)->next;
        }
        *node = l1?l1:l2;
        return head;
    }
    //一级指针做法
    template<typename T>
    ListNode<T>* mergeTwoLists2(ListNode<T>*l1,ListNode<T>*l2)
    {
        if(!l1 || !l2 )
            return l1?l1:l2;
        ListNode<T>*head = nullptr ;
        ListNode<T>*node =nullptr;//
        while( l1!=nullptr && l2!=nullptr )
        {
            if( l1->val<l2->val )
            {
                if(node==nullptr)
                    node = l1;
                else
                    node->next = l1;
                l1 = l1->next;
            }
            else
            {
                if(node==nullptr)
                    node = l2;
                else
                    node->next = l2;
                l2 = l2->next;
            }
            if(head == nullptr )
                head = node;
            else
                node = node->next;
        }
        node->next = l1?l1:l2;
        return head;
    }

};
RegistAlgorithm(MergeOrderedList)
/*
    输入一个链表，反转链表
*/
class ReserverList:public AlgorithmSolution{
    public:

    ReserverList()
    {
        m_algorithmName = "反转链表 ";
    }
    void Solution()override
    {
        List<int>l;
        for( int i=0;i<30;++i )
            l.insert(i);
        std::cout<<"原始链表: "<<std::endl;
        l.print();
        List<int>l2(reverseList(l.head()));
        std::cout<<"反转后的链表"<<std::endl;
        l2.print();
    }
    private:
    template<typename T>
    ListNode<T>* reverseList(ListNode<T>*head )
    {
        ListNode<T>* before =nullptr,*temp;
        while( head )
        {
            temp = head->next;
            head->next = before;
            before = head;
            head = temp;
        }
        return before;
    }
    
};
RegistAlgorithm(ReserverList)
/*
给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。

如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和
输入：(2 -> 4 -> 3) + (5 -> 6 -> 5)
输出：7 -> 0 -> 9
原因：342 + 565 = 907
//感觉这个得反转链表，然后计算。。。。
*/
class AddTwoNumberList:public AlgorithmSolution{
    public:
    AddTwoNumberList()
    {
        m_algorithmName="两数相加 两个链表代表的整数相加";
    }
    void Solution()override
    {
        List<int> l1{2,4,3};
        List<int> l2{5,6,5};
        std::cout<<"原始链表: "<<std::endl;
        l1.print();
        l2.print();
        List<int> ret;
        ret.head__() = addTwoNumbers(l1.head(),l2.head());
        std::cout<<"相加之后的链表: "<<std::endl;
        ret.print();
    }
    template<typename T>
    ListNode<T>* addTwoNumbers(ListNode<T>*l1,ListNode<T>*l2 )
    {
        if(!l1 || !l2 )
            return l1?l1:l2;
        ListNode<T>*head = nullptr;
        ListNode<T>**node = &head;
        int k=0;
        while( l1 && l2 )
        {
            *node = new ListNode<T>();
            int temp = k+l1->val+l2->val;
            (*node)->val = temp%10;
            k = temp/10;
            node = &((*node)->next);
            l1 = l1->next;
            l2 = l2->next;
        }
        decltype(l1) left_list = l1?l1:l2;
        while(left_list)
        {
            *node = new ListNode<T>();
            int temp = k+left_list->val;
            (*node)->val = temp%10;
            k = temp/10;
            node = &((*node)->next);
            left_list = left_list->next;
        }
        if(k>0)
        {
           *node = new ListNode<T>();
           (*node)->val = k;
            node = &((*node)->next);
        }
        *node = NULL;
        return head;
    }
};
RegistAlgorithm(AddTwoNumberList);

/*
给你一个链表，每 k 个节点一组进行翻转，请你返回翻转后的链表。

k 是一个正整数，它的值小于或等于链表的长度。

如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。

给你这个链表：1->2->3->4->5

当 k = 2 时，应当返回: 2->1->4->3->5

当 k = 3 时，应当返回: 3->2->1->4->5

说明：

你的算法只能使用常数的额外空间。
你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。
*/
template<size_t N>
class ReverseKGroup:public AlgorithmSolution{
    public:
    ReverseKGroup()
    {
        m_algorithmName = "K个一组 翻转链表";
    }
    void Solution()override
    {
        List<int> l={1,2,3,4,5};
        std::cout<<"原始链表: ";
        l.print();
        List<int> ret(reverseKGroup(l.head(),N) );
        std::cout<<" 经过 "<<N<<" 个一组 翻转后，得到 ";
        ret.print();
    }
    template<typename T>
    ListNode<T>* reverse(ListNode<T>*n )
    {
        ListNode<T>*head = nullptr;
        ListNode<T>* temp = nullptr;
        while( n )
        {
            temp = n->next;
            n->next = head;
            head= n;
            n = temp;
        }
        return head;
    }
    //常数空间
    template<typename T>
    ListNode<T>* reverseKGroup(ListNode<T>*head, int k )
    {
        if(head==NULL || k<0 )
            return NULL;
        if(k<=1)
            return head;
        decltype(head) last_end=nullptr;
        decltype(head) new_head=nullptr;
        decltype(head) beg =head,end=nullptr;
        int idx=0;
        while(head)
        {
            end = head;
            head = head->next;
            idx++;
            if(idx == k )
            {
                end->next = nullptr;
                if(new_head == nullptr )
                    new_head = reverse(beg);
                else
                    last_end->next = reverse(beg);
                last_end = beg;//原来的beg反转后变成了最后一个元素
                beg = head;
                idx=0;
            }
        }
        if(last_end)
            last_end->next = beg;
        return new_head;
        
    }
};
RegistAlgorithm(ReverseKGroup<3>)