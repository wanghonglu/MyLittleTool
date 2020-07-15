CC=g++
CPP=g++ -std=c++11
Lib=-lm
OBJ=$(patsubst %.c, %.o, $(wildcard *.c))
CPPOBJ=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
All:Sort
Sort:$(OBJ) $(CPPOBJ)
	g++  -g $(Lib) -o $@ $^
	@chmod +x Sort
#这个命令会展开为一个个 .c编译.o 这样就可以未改动的文件不编译了
$(OBJ):%.o:%.c
	$(CC) -g -c $(Lib) -o $@ $^ 
$(CPPOBJ):%.o:%.cpp
	$(CPP) -g -c $(Lib) -o $@ $^
clean:
	@rm -rf Sort *.o
.PHONY:All clean
