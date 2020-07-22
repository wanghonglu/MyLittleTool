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
 *
 * */
#include<stdlib.h>
#include<stdio.h>
typedef struct ListNode {
    int val;
    struct ListNode *next;
}ListNode;
struct ListNode* getKthFromEnd(struct ListNode* head, int k){
    if( head == NULL || k<=0 )
        return NULL;

    struct ListNode* f=head, *s=head;
    int temp = k;

    while( f && temp-- )
        f = f->next;
    if( f == NULL && temp == 0 )
        return s;

    if( f == NULL && temp>0 )
        return NULL;

    while( f )
    {
        s = s->next;
        f = f->next;
    }
    return s;
}
/*
	还有一个类似的题
	给定一个链表，删除链表的倒数第 n 个节点，并且返回链表的头结点。

	示例：

	给定一个链表: 1->2->3->4->5, 和 n = 2.

	当删除了倒数第二个节点后，链表变为 1->2->3->5.

*/
ListNode* removeNthFromEnd(ListNode* head, int n) {
	if (head == NULL || n < 1)
		return NULL;
	ListNode* f = head, *s = head;
	int i = 0;
	for (i = 0; i < n&&f; i++)
	{
		f = f->next;
	}
	if (f == NULL && i == n)
	{
		ListNode* temp = head;
		head = head->next;
		free( temp);
		return head;
	}
	if (f == NULL)
		return NULL;
	ListNode* pre = NULL;
	while (f)
	{
		f = f->next;
		pre = s;
		s = s->next;
	}
	pre->next = s->next;
	free( s );
	return head;


}
int main(int argc,char**argv)
{
    struct ListNode* n1  = (struct ListNode* )malloc( sizeof( struct ListNode ));    
    struct ListNode* n2  = (struct ListNode* )malloc( sizeof( struct ListNode ));    
    struct ListNode* n3  = (struct ListNode* )malloc( sizeof( struct ListNode ));    
    n1->val=1;
    n1->next = n2;
    n2->val=2;
    n2->next=n3;
    n3->val=3;
    n3->next = NULL;

    struct ListNode* temp = getKthFromEnd(n1, atoi(argv[1]) );
    if( temp !=NULL )
    {
        printf("%d ", temp->val );
    }
    free( n1 );
    free( n2 );
    free( n3 );
}
