CC=g++
Opt= -g -std=c++11
all:WHL_dataStruct
#下面的dep也会产生目标 所以all要放在它上面
Dep=$(wildcard *.d)
-include $(Dep)
OBJ=$(patsubst %.cpp, %.o, $(wildcard *.cpp ))
Def= -D_UNIT_TEST_
Lib= -lpthread
WHL_dataStruct:$(OBJ)
	$(CC) $(Opt) -o $@ $^ $(Lib) 
$(OBJ):%.o:%.cpp 
	$(CC) $(Def) -c $(Opt) -o  $@  $< -MMD -MF $*.d -MP 
clean:
	@rm WHL_dataStruct *.o *.d
.PHONY:clean DataStruct
