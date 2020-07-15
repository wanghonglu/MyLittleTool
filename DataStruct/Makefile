CC=g++
Opt= -g -std=c++11
OBJ=$(patsubst %.cpp, %.o, $(wildcard *.cpp ))
All=DataStruct
Def= -D_UNIT_TEST_
DataStruct:$(OBJ)
	$(CC) $(Def) $(Opt) -o $@ $^
$(OBJ):%.o:%.cpp
	$(CC) $(Def) -c $(Opt) -o  $@ $^
clean:
	@rm DataStruct *.o
.PHONY:clean DataStruct
