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
#ifndef __FIND_KTH_FROM_END
#define __FIND_KTH_FROM_END
#include<stdlib.h>
#include<stdio.h>
#include "Algorithm_test.hpp"
class ListGetKthFromEnd:public AgorithmSolution
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
        std::cout<<"倒数第 "<<k<<"个值为: "<<node->val_<<std::endl;
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
            fast = fast->next_;
        if( fast == nullptr && i == k )
            return slow;
        if( fast == nullptr )
            return nullptr;
        while( fast!=nullptr )
        {
            slow = slow->next_;
            fast = fast->next_;
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
            fast = fast->next_;
        
        do
        {
            if( fast == nullptr && i!=k )
                return ;
            while( fast!=nullptr )
            {
                fast = fast->next_;
                slow = slow->next_;   
            }
            l.erase(slow);
        }while(0);

    }
};
/*
 * 判断链表是否有环并且找到环所在的位置
 * 判断是否有环 可以通过快慢指针,快指针每次走2步  慢指针每次走1步
 * 只要有环 进入环后,快指针速度比慢指针快,那么快指针最终会追上慢指针的
 * 当追上的时候 此时慢指针总共走了 N步的话 那么快指针就是2N步
 * 假设他们相遇在M点,那么此时把快指针再从头开始走,一次一步,满指针也继续从M点走一次也一步
 * 因为他俩速度一样, 那么他俩走N步必然会再次在M点集合,此时 他们第一次相遇的点 就是环的位置
 * */
class FindListCircle:public AgorithmSolution
{
    public:
    FindListCircle()
    {
        m_algorithmName="查找链表是否有环，并找到环的位置";
    }
    void Solution()override
    {
        AgorithmSolution::Solution();
        List<int> l={1,2,3,4,5,6,7,8};

        auto node = l.head();
        node = node->next_->next_;

        auto temp = l.head();
        while( temp->next_ !=nullptr )
            temp = temp->next_;
        
        //造一个环 ，让8的next指针指向3
        temp->next_ = node;

        auto test=[this](ListNode<int>*head){
            auto temp = findListCircle(head);
            if( temp )
            {
                std::cout<<"链表有环，环的节点的值为 "<<temp->val_<<std::endl;
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
        while( slow && fast && fast->next_ )
        {
            if( slow == fast && slow !=head )
            {
                bhaveCircle = true;
                break;
            }
            slow = slow->next_;
            fast = fast->next_->next_;
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
            fast = fast->next_;
            slow = slow->next_;
        }
        //这里多加一步，不然这个有环的链表就析构不了了，在环里的slow指针上一次指向的就是这个环
        //让它指向nullptr保证析构掉
        parent->next_ = nullptr;
        return fast;
    }
};
/*
输入两个递增排序的链表，合并这两个链表并使新链表中的节点仍然是递增排序的。
*/
class MergeOrderedList:public AgorithmSolution
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
        for( int i=0;i<20;i++ )
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
        List<int>l3(mergeTwoLists(l1.head(),l2.head()));

        std::cout<<"合并后的链表"<<std::endl;
        l3.print();
    }
    private:
    template<typename T>
    ListNode<T>* mergeTwoLists(ListNode<T>*l1,ListNode<T>*l2)
    {
        if(!l1 || !l2 )
            return l1?l1:l2;
        ListNode<T>*head,**node = &head;
        while( l1!=nullptr && l2!=nullptr )
        {
            if( l1->val_<l2->val_ )
            {
                *node = l1;
                l1 = l1->next_;
            }
            else
            {
                *node = l2;
                l2 = l2->next_;
            }
            node = &(*node)->next_;
        }

        while(l1!=nullptr )
        {
            *node = l1;
            l1 = l1->next_;
            node =&(*node)->next_;
        }
        while(l2!=nullptr )
        {
            *node = l2;
            l2 = l2->next_;
            node =&(*node)->next_;
        }
        return head;
    }

};
#endif