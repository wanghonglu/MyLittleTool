#!/bin/sh
if [ ! -n "$1"  ];then
    echo "需要输入参数"
    exit
fi
C_FILE=$1
OBJ_FILE=${C_FILE%.*}
gcc jump_2_context_2.s co_ctx.c -lpthread -g $1 -o $OBJ_FILE
