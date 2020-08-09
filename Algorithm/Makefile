CC=g++
Opt= -g -std=c++11
all:WHL_AlgorithmTest
#下面的dep也会产生目标 所以all要放在它上面
Dep=$(wildcard *.d)
-include $(Dep)
OBJ=$(patsubst %.cpp, %.o, $(wildcard *.cpp ))
Def= -D_UNIT_TEST_
WHL_AlgorithmTest:$(OBJ)
	$(CC) $(Opt) -o $@ $^ 
$(OBJ):%.o:%.cpp 
	$(CC) $(Def) -c $(Opt) -o  $@  $< -MMD -MF $*.d -MP 
clean:
	@rm WHL_AlgorithmTest *.o *.d
.PHONY:clean WHL_AlgorithmTest
