#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct Stack{
    int*     val_;
    size_t   size_;
    size_t   capacity_;
}Stack;

Stack* 
InitStack(size_t  size )
{
    Stack *st = (Stack*)malloc(sizeof(Stack));
    st->size_=0;
    st->capacity_=size;
    st->val_=(int*)malloc(sizeof(int)*size);
    return st;
}
int
IsEmpty(Stack* st )
{
    return st->size_==0;
}
int 
IsFull(Stack* st )
{
    return st->size_ >= st->capacity_;
}
int 
Top(Stack* st )
{
    if( !IsEmpty(st) )
        return st->val_[st->size_-1];
    return -1;
}
void 
Push(Stack* st, int val )
{
    if( !IsFull(st) )
        st->val_[st->size_++]=val;
    return;
}
int 
Pop(Stack*st)
{
    if( IsEmpty(st) )
        return -1;
    int val = st->val_[--st->size_];
    return val;
}
void ClearStack(Stack*st)
{
    free(st->val_);
    free(st);
}
void PrintStack(Stack *st)
{
    while( !IsEmpty(st) )
    {
        printf("%d ", Top(st));
        Pop(st);
    }
    printf("\n");
}
/*
 * 给出一个矩形统计图，它的每个矩形的宽度都为1，高度是题目所给。要你求出这个矩形图中最大面积的长方形。
 * 比如矩阵
 * 矩形统计图的数据为 [4, 3, 2, 5, 6]
 * 则最大矩形面积就是 5*5=25
 * 可以用单调栈来解决 
 * 思路是: 当前元素左右两边扩展 遇到比自己大的 就继续扩展 比自己小的就截止即元素所在最大面积由最小的决定
 * 构造单调递减栈( 即栈顶大 而栈底小 )
 * 如果当前元素大于等于栈顶入栈   则入栈 否则栈顶出栈,此时需要计算栈顶所在最大矩形面积 
 * 计算方法为 pop出来该计算元素,新的栈顶 即该计算元素的左边界 计算元素的右边界为遍历元素 有了两个边界 面积自然好算
 * */
int GetMaxArray( int *heights, size_t heightsSize  )
{
 #define Max(a,b) ((a)>(b)?(a):(b))
    Stack *st=InitStack(heightsSize);
    int maxArea=0,idx=0;
    for(;idx<heightsSize;idx++ )
    {
        while( !IsEmpty(st) && heights[idx]< heights[Top(st)] )
        {
           int current = Top(st);
           Pop(st);
           //整个宽度就是左边+右边+1(1是自身的宽度)  面积 宽度x高度
           int left = IsEmpty(st)?current:(current-Top(st)-1);
           int right = idx-current-1;
           maxArea = Max( maxArea, (left+right+1)*heights[current] );
        }
        Push(st, idx);
    }
    //计算剩余栈内的最大面积
    if( IsEmpty(st) )
    {
        ClearStack(st);
        return maxArea;
    }

    while( !IsEmpty(st) )
    {
        int current = Top(st);
        Pop(st);
        int left = IsEmpty(st)?current:(current-Top(st)-1);
        int right =heightsSize-1-current; 
        maxArea = Max(maxArea, (left+right+1)*heights[current]);
    }

    ClearStack(st);
    return maxArea;

#undef  Max

}
/*
 * 描叙：有n个人站队，所有的人全部向右看，个子高的可以看到个子低的发型，给出每个人的身高，问所有人能看到其他人发现总和是多少。
 * 输入：4 3 7 1
 * 输出：2
 * 解释：个子为4的可以看到个子为3的发型，个子为7可以看到个子为1的身高，所以1+1=2
 * 因为是向右看 所以可以构造一个单调递增栈( 单调递增指的是 出栈的时候 元素是单调递增的 , 即栈顶小 而栈顶大 )
 * 然后判断 如果元素小于栈顶 则入栈
 * 否则弹出栈顶 注意这里栈顶元素能看到的人是" 当前元素的下标-栈顶元素的下标-1 "
 * 如果他俩相邻 则栈顶元素谁都看不到
 * 最后还需要把栈里的所有元素弹出来再计算  此时栈里元素能看到的人数是  最后一个人下标-当前人下标-1
 * */
int CanLookRight( int *array, size_t len  )
{
    int result =0;
    Stack *st = InitStack(len);
    int idx=0;
    for(  ;idx<len;++idx )
    {
        while( !IsEmpty(st) && array[idx]>= array[Top(st)]  )
        {
            int topIdx = Top(st);
            Pop(st);
            result +=( idx-topIdx-1 );
        }
        Push(st, idx);
    }
    while( !IsEmpty(st) )
    {
        int topIdx = Top(st);
        Pop(st);
        //此时的idx代表最后一个人 意思是 topIdx能看到的人从自己开始到最后一个截至
        result +=( idx-topIdx-1 );
    }
    ClearStack(st);
    return result;
}
/*
 *  下一个最高温度问题
 *  给定一个数组,代表每天的问题, 算出一个数组每一项代表下一个更高问题要等待的天数
 *  比如输入temperatures = [73, 74, 75, 71, 69, 72, 76, 73]，你的输出应该是 [1, 1, 4, 2, 1, 1, 0, 0]。
 *  因为要找的是比自己大的元素 所以这里需要构建的是递增栈
 *  每次由比栈顶大的数 则表明找到了 当前栈顶的下一个更大的温度 比栈顶小则入栈
 *
 * */
int* dailyTemperatures(int* T, int TSize, int* returnSize){
    if( T== NULL  )
        return NULL;
    Stack *st=InitStack(TSize);
    *returnSize = TSize;
    int *array = (int*)malloc(TSize*sizeof(int));
    memset( array, 0, TSize*sizeof(int) );
    int idx=0;
    for(;idx<TSize;idx++ )
    {
       while( !IsEmpty(st)&& T[idx]>T[Top(st)] ) 
       {
            int current = Top(st);
            Pop(st);
            array[current]=idx-current;
       }
       Push(st, idx);
    }
    //剩下的就不用算了 都是0
    ClearStack(st);
    return array;
}
/*
 *  接雨水问题,跟矩阵类似,给定宽度为1 不同长度的柱子 求最大能接的雨水的量
 *  比如输入 [1,0,0,2] 那么能接的雨水就是2个单位
 *  输入[0,1,0,2,1,0,1,3,2,1,2,1] 则能接雨水就是6个单位 可画图自己分析
 *  这个跟矩形面积类似  但又不同
 *  当前元素应该找左边 比它小的最大的元素
 *  故应该建立一个递减栈 栈顶大而栈底小
 *  
 *
 * */
int trap(int* height, int heightSize)
{
    if( height == NULL  )
        return 0;
    Stack *st = InitStack(heightSize);

    int idx=0 ,total=0;
    for(;idx<heightSize;idx++ )
    {
        while(!IsEmpty(st) && height[idx] > height[Top(st)])
        {
            int top = Top(st);
            Pop(st);
            if( IsEmpty(st) )
                continue;
            //栈内的元素比current 大  即新的top左边比current大的 idx是右边比current大的
            int len = idx-Top(st)-1;
            int h = height[Top(st)]<height[idx]?height[Top(st)]:height[idx] - height[top];
            int area = len*h;
            printf("current :%d area:%d\n", idx, area );
            total+=area;
        }
        Push(st,idx);
    }
    //剩下的存不下水
    ClearStack(st);
    return total;
    
}
void Print(int*a, size_t len)
{
    int idx=0;
    for(;idx<len;idx++)
        printf("%d ", a[idx]);
    printf("\n");
}
#define IntArraylen(a) sizeof(a)/sizeof(int)
int main()
{

    //int a[]={4,3,2,5,6};
   // int a[]={5,4,1,2};
    //int a[]={2,1,5,6,2,3};
//    int a[]={73,74,75,71,69,72,76,73};
    int a[]={0,1,0,2,1,0,1,3,2,1,2,1};
//    int a[]={1,0,0,1};
    printf("输入的数组\n");
    Print(a, IntArraylen(a));
    //GetMaxArray(a,IntArraylen(a));

    //向右看到的所有人数总和
    printf( "向右能看到的所有人数: %d\n", CanLookRight(a, IntArraylen(a)) );
    
    //最大矩形面积
    printf("最大矩形面积:%d\n",GetMaxArray(a,IntArraylen(a)) );

    //更高温度问题
    int b=0;
    int *num = dailyTemperatures(a, IntArraylen(a),&b );

    printf("下一个更高温度:");
    Print(num, b);
    free(num);

    //存储雨水量
    printf("存储雨水量:%d\n", trap(a, IntArraylen(a)));

    return 0;
}

