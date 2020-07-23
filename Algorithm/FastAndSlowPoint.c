#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct Node{
    int val_;
    struct Node *next_;
}Node;

typedef struct List{
    Node *head_;
    size_t  len_;
}List;

List*
InitList()
{
    List *l = (List*)malloc(sizeof(List));
    l->head_=NULL;
    l->len_=0;
    return l;
}
void 
Construct(List* l, int *val, size_t len)
{
    if( l==NULL || val == NULL ) return;
    int i=0;

    for(;i<len;i++ )
    {
       Node *n = (Node*)malloc(sizeof(Node));
       n->val_=val[i];
       n->next_=l->head_;;
       l->head_ = n;
       l->len_++;
    }
}
void Push(List*l, Node* t )
{
    t->next_ = l->head_;
    l->head_=t;
    l->len_++;
}
void
ClearList(List* l)
{
    Node* ptr = l->head_, *tmp;
    while( ptr  )
    {
        tmp = ptr->next_;
        free(ptr);
        ptr = tmp;
    }
    free(l);
}
void 
PrintList(List*l )
{
    Node* ptr=l->head_;
    while( ptr )
    {
        printf("%d ", ptr->val_);
        ptr = ptr->next_;
    }
    printf("\n");
}
/*
 * 判断链表是否有环并且找到环所在的位置
 * 判断是否有环 可以通过快慢指针,快指针每次走2步  慢指针每次走1步
 * 只要有环 进入环后,快指针速度比慢指针快,那么快指针最终会追上慢指针的
 * 当追上的时候 此时慢指针总共走了 N步的话 那么快指针就是2N步
 * 假设他们相遇在M点,那么此时把快指针再从头开始走,一次一步,满指针也继续从M点走一次也一步
 * 因为他俩速度一样, 那么他俩走N步必然会再次在M点集合,此时 他们第一次相遇的点 就是环的位置
 * */
Node* 
FindCircle( List *l )
{
    Node *f,*s;
    int bHavecircle=0;
    f = s = l->head_;
    while( f && f->next_ )
    {
        f = f->next_->next_;
        s = s->next_;
        if( s == f )
        {
            bHavecircle = 1;
            break;
        }
    }
    if( !bHavecircle  )
        return NULL;

    f = l->head_;
    while( f && s )
    {
        f = f->next_;
        s = s->next_;
        if( f == s  )
            break;
    }
    return f;
}
#define IntArrayLen(a) sizeof(a)/sizeof(int)
int FastSlowPoint()
{
    List *l=InitList();
    Node* n1 = (Node*)malloc(sizeof(Node));
    n1->val_=100;
    Push(l,n1);
    Node* n2 = (Node*)malloc(sizeof(Node));
    n2->val_=200;
    Push(l,n2);
    Node* n3 = (Node*)malloc(sizeof(Node));
    n3->val_=300;
    int a[]={1,2,3,4,5,56,7};
    Construct(l, a, IntArrayLen(a));
    Push(l,n3);
    int b[]={11,22,33,44,55,566,77};
    Construct(l, b, IntArrayLen(b));
    //PrintList(l);
    //构造环
    n1->next_ = n3;

    Node * node = FindCircle(l);

    if( node!=NULL  )
    {
        printf("有环 环的位置:%d\n", node->val_);
    }

    //ClearList(l);
}

