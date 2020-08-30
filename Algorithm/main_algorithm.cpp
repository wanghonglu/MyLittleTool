#include"BaseAlgorithm.hpp"
size_t AlgorithmSolution::s_index=0;
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