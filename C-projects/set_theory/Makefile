#set_project Makefile: expected location "project_1/"

#directory variables setup
SRC		= src
SRCS	= $(wildcard $(SRC)/*.c)
HEADERS = $(wildcard $(SRC)/*.h)

OP_DIR	= set_ops
SET_OPS	= $(wildcard $(OP_DIR)/*.c)

SF_DIR	= set_funcs
SET_FNS	= $(wildcard $(SF_DIR)/*.c)

MSC_DIR	= misc_funcs
MSC_FNS	= $(wildcard $(MSC_DIR)/*.c)

#compiler variables setup
CC_ALL	:= gcc -g -Wall -Wextra -Wformat=2 -Wnull-dereference -Wpedantic -O3
CC_DBG	:= gcc -g -Wall -Wextra -Wformat=2 -Wnull-dereference -Wpedantic

all:	$(SRCS) $(HEADERS) $(SET_OPS) $(SET_FNS) $(MSC_FNS)
		$(CC_ALL) $^ -o $(SRC)/set_practical

debug:	$(SRCS) $(HEADERS) $(SET_OPS) $(SET_FNS) $(MSC_FNS)
		$(CC_DBG) $^ -o $(SRC)/set_prac-debug

clean:
		rm -rf all
