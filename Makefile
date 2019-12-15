CC =g++
FLAG= -g -Wall 
Lib=-lpthread  -ldl -lboost_thread -lboost_system 
INCLUDES= -I/home/wanghl/boost/boost_1_72_0
OBJ=main.o log.o def.o  ConsulClient.o 
TARGET=TESTBIN
${TARGET}:${OBJ}
	${CC} ${FLAG}  ${OBJ} ${Lib} -o ${TARGET}
.cpp.o:
	${CC} ${FLAG} -c $< -o  $@
clean:
	rm -rf  *.o ${TARGET}
