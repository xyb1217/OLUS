##############################项目路径和程序名称#############################
PRO_NAME=comm

DIR=$(shell pwd)
SRC_DIR=$(DIR)
INC_DIR=-I$(DIR)/../ -I/usr/local/include 

######依赖库#####
LIBS_DIR=
LIBS_NAME=
LIBS=$(LIBS_DIR) $(LIBS_NAME)

LIB_NAME=lib$(PRO_NAME).a
#BIN_NAME=

SRC_LIB=$(DIR)/$(LIB_NAME)
SRC_INC=$(DIR)/*.h
DEST_LIB_DIR=$(DIR)/../../lib/


##############################OBJ文件及路径##################################
EXT=cpp
SRCS=$(wildcard $(SRC_DIR)/*.$(EXT))
OBJS=$(SRCS:.cpp=.o)


#############################编译选项及编译器################################
CC=g++
#CFLAGS=-Wall -m64 -W -g -fPIC -finput-charset=gb2312
CFLAGS=-Wall -m64 -W -g -O2 -fPIC -D NDEBUG
LDFLAGS=-shared -fPIC


###############################编译目标######################################
.PHONY: all clean rebuild
all:$(OBJS) 
	ar -r $(LIB_NAME) $(OBJS)
	mkdir -p $(DEST_LIB_DIR)
	cp -rf $(SRC_LIB) $(DEST_LIB_DIR)

$(OBJS):%.o:%.cpp
	$(CC) $(CFLAGS) $(INC_DIR) -c  $< -o $@

rebuild: clean all

clean:
	rm -rf $(OBJS) $(SRC_LIB) $(DEST_LIB_DIR)/$(LIB_NAME) $(DEST_INC_DIR)/*.h


