ALLFILE=$(wildcard *.c)
TargetFile=$(wildcard test_*.c)

TargetFileCOBJ=$(patsubst %.c,%.o, $(TargetFile))
all:show
.PHONY:all
#all:show
#有用的c文件 包含具体实现
#SRC_C_FILE=process_scheduler.c coroutine_ctx.c
SRC_C_FILE=$(filter-out $(TargetFile),$(ALLFILE) )

SRC_C_FILE_OBJ=$(patsubst %.c,%.o,$(SRC_C_FILE))
#ASM_FILE=coroutine_ctx_swap.s
ASM_FILE=$(wildcard *.s)
ASM_FILE_OBJ=$(patsubst %.s,%.o,$(ASM_FILE))

CC= gcc -g -Wall
LIB= -lpthread


TMP_TargetBin=$(patsubst %.o,%,$(TargetFileCOBJ))
#统一转化成大写作为可执行文件的名字
TargetBin=$(shell echo $(TMP_TargetBin)|tr 'a-z' 'A-Z')
$(TargetBin):$(SRC_C_FILE_OBJ) $(ASM_FILE_OBJ)


#TargetFileCOBJ = test_recursive_corotine.o test_mutiple_thread.o test_max_stack_corotine.o
define COMPILE_EACH
.PHONY:$(1)
$(1):$(shell echo $(1)|tr 'A-Z' 'a-z').o $(SRC_C_FILE_OBJ) $(ASM_FILE_OBJ)
	@echo "building $1 ... "
	$(CC) $(LIB) $(shell echo $(1)|tr 'A-Z' 'a-z').o $(SRC_C_FILE_OBJ) $(ASM_FILE_OBJ) -o $1

endef
$(foreach one, $(TargetBin),$(eval $(call COMPILE_EACH, $(one))))

$(TargetFileCOBJ):%.o:%.c
	$(CC) -c -o $@ $^
$(SRC_C_FILE_OBJ):%.o:%.c
	$(CC) -c -o $@ $^
$(ASM_FILE_OBJ):%.o:%.s
	$(CC) -c -o $@ $^
clean:
	@rm -rf *.o
	$(foreach one,$(TargetBin),rm -rf $(one))
show:
	@echo "选择要编译的模块 $(TargetBin) $(SRC_C_FILE_OBJ) $(ASM_FILE_OBJ)"

