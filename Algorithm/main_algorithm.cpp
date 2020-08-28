#include"BaseAlgorithm.hpp"
#include "KMP_Algorithm.hpp"
#include "BaseAlgorithm.hpp"
#include "CommonDataStruct.hpp"
#include  <cstring>
#include "Nqueue_Quest.hpp"
#include "GetMinCoinQuest.hpp"
#include "find_unique_number.hpp"
#include "ListAgorithm.hpp"
#include "Hanoi.hpp"
#include "maxSubArray.hpp"
#include "TopKFrequent.hpp"
#include "Sigle_Stack.hpp"
#include "BinaryTreeALgorithm.hpp"
#include "KMP_Algorithm.hpp"
int main(int argc,char**argv )
{
    int opt =-1;
    try
    {
       main_datastruct_test();
       if( argc>1 )
            opt = atoi(argv[1]);
       AlgorithmSolution* test = GetOneTest(opt);
       test->Solution();
    }
    catch(const std::exception& e)
    {
        GetHelp();
        std::cerr << e.what() << '\n';
    }
    
}