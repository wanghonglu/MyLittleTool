TMPSRC=amqp_consumer.c 
TMP=temp.o
INCLUDE=-I./ -I./../librabbitmq/ -I./../librabbitmq/unix
CC =gcc 
DEFINE=-DHAVE_POLL -DAMQ_PLATFORM=\"linux\"
#DEFINE=
Target=AAAA
${Target}:unity_build.o utils.o platform_utils.o  ${TMP}
	${CC} ${INCLUDE}  $^ -o $@
utils.o:utils.c
	${CC} ${INCLUDE} -c $^ -o $@
unity_build.o:./../librabbitmq/unity_build.c
	${CC} ${INCLUDE} ${DEFINE} -c $^ -o $@
platform_utils.o:./unix/platform_utils.c
	${CC}  -c $^ -o $@
${TMP}:${TMPSRC}
	${CC} ${INCLUDE}  -c $^ -o $@ 
clean:
	rm -rf ${Target} ${TMP}

