#!/bin/sh
if [ ! -n "$1"  ];then
    echo "需要输入参数"
    exit
fi
C_FILE=$1
OBJ_FILE=${C_FILE%.*}
#转换成大写
OBJ_FILE=`echo $OBJ_FILE|tr 'a-z' 'A-Z'`
gcc -g jump_2_context_2.s co_ctx.c process_scheduler.c -lpthread $1 -o $OBJ_FILE
