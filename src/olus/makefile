
##############################项目路径和程序名称#############################
PRO_NAME=pre_sign

DIR=$(shell pwd)
SRC_DIR=$(DIR)
INC_DIR=-I$(DIR) -I$(DIR)/.. -I/usr/local/include/ccm \
		-I/usr/local/include/hiredis -I/usr/local/mysql/include\
		-I/usr/local/ssl/include -I/usr/include/mysql

######依赖库#####-lstdc++ 
LIBS_DIR=-L/usr/lib64 -L$(DIR)/../../lib -L/usr/local/lib \
		-L/usr/local/ssl/lib -L/usr/local/mysql/lib -L/usr/lib64/mysql

LIBS_NAME=-lcomm -lim -lfre_sign -lins_sign -lins_sync -lcache -lwossl \
		-lccm_cm -lccm_im -lccm_db -levent -lxml2 -lcrypto -lssl -lpthread\
		-levent -levhtp -luuid -lstdc++ -lmysqlclient -lhiredis -ljsoncpp
		
LIBS=$(LIBS_DIR) $(LIBS_NAME)

BIN_NAME=$(PRO_NAME)

SRC_BIN=$(DIR)/$(BIN_NAME)
DEST_BIN_DIR=$(DIR)/../../bin


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
	$(CC) -o $(SRC_BIN) $(OBJS) $(CFLAGS) $(LIBS)
	mkdir -p $(DEST_BIN_DIR)
	cp -rf $(SRC_BIN) $(DEST_BIN_DIR)


$(OBJS):%.o:%.cpp
	$(CC) $(CFLAGS) $(INC_DIR) -c  $< -o $@

rebuild: clean all

clean:
	rm -rf $(OBJS) $(BIN_NAME) $(DEST_BIN_DIR)/$(BIN_NAME) 


