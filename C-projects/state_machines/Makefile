#fsm_practical Makefile: expected location "project_2/" 

#directory variables setup
SRC		= src
SRCS	= $(wildcard $(SRC)/*.c)
HEADERS = $(wildcard $(SRC)/*.h)

ST_DIR	= craft_states
STATES	= $(wildcard $(ST_DIR)/*.c)

MSC_DIR	= misc_funcs
MSC_FNS	= $(wildcard $(MSC_DIR)/*.c)

#compiler variables setup
CC_ALL	:= gcc -Wall -Wextra -Wformat=2 -Wnull-dereference -Wpedantic -O3
CC_DBG	:= gcc -Wall -Wextra -Wformat=2 -Wnull-dereference -Wpedantic -g3

all:	$(SRCS) $(HEADERS) $(STATES) $(MSC_FNS)
		$(CC_ALL) $^ -o $(SRC)/fsm_practical

debug:	$(SRCS) $(HEADERS) $(SET_OPS) $(SET_FNS) $(MSC_FNS)
		$(CC_DBG) $^ -o $(SRC)/fsm-debug

clean:
		rm -rf all
		rm -rf debug
