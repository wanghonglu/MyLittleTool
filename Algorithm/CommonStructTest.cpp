#include "CommonDataStruct.hpp"
void ListTest()
{
       List<int> l={1,2,3,4,5,6,7};
       auto node = l.head();
       l.print();
       l.erase(node);
       l.print();
       while(node->next)
       {
           node = node->next;
       }
       l.erase(node);
       l.print();
       l.insert(100);
       l.insert(200);
       l.print();
}
void stack_test()
{
    Stack<int> st;
    for( int i=0;i<1000;i++ )
        st.push(i);
    std::cout<<st.top()<<std::endl;
    assert(st.top() == 999 );
    while(!st.empty())
        st.pop();
    std::cout<<"stack test end "<<std::endl;
}
void Queue_test()
{
    Queue<int>  q;
    q.push(100);
    q.push(200);
    assert(q.front() == 100 && q.back() ==  200 );
    q.pop();
    for( int i=0;i<10;i++ )
    {
        q.push(i);
    }
    q.print();
}
void priority_queue_test()
{
    PriorityQueue<int> q={90,1,1000,-1,20,3,-10,1};
    while(!q.empty())
    {
        std::cout<<q.top()<<" ";
        q.pop();
    }
    std::cout<<std::endl;
    int number=1000;
    for( int i=0;i<number;i++ )
        q.push(i);
    std::cout<<"push end "<<std::endl;
    for( int i=number-1;i>=0;i-- )
    {
        assert(i == q.top() );
        q.pop();
    }
    std::cout<<__FUNCTION__<<" test end "<<std::endl;
}
void binarytree_test()
{
    BinaryTree<int> b={-10,9,20,INT_MIN,INT_MIN,15,7};
    b.levelOrder();
}
void main_datastruct_test()
{
    //ListTest();
    //stack_test();
    //priority_queue_test();
    //Queue_test();
    //binarytree_test();
}