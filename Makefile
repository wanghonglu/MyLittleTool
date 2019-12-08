CC =g++
FLAG= -g -Wall 
Lib=-lpthread 
OBJ=main.o log.o def.o 
TARGET=TESTBIN
${TARGET}:${OBJ}
	${CC} ${FLAG}  ${OBJ} ${Lib} -o ${TARGET}
.cpp.o:
	${CC} ${FLAG} -c $< -o  $@
clean:
	rm -rf  *.o ${TARGET}
