#calc-assign Makefile: expected location is "claytor/"

#directory variables setup
SRC		= src
SRCS	= $(wildcard $(SRC)/*.c)
HEADERS = $(wildcard $(SRC)/*.h)

MATH	= math_funcs
MATHFNS	= $(wildcard $(MATH)/*.c)

MISC	= misc_funcs
MISCFNS	= $(wildcard $(MISC)/*.c)

STACK	= stack_funcs
STAKFNS = $(wildcard $(STACK)/*.c)

#compiler variables setup
CC_ALL	:= gcc -Wall -Wextra -Wformat=2 -Wnull-dereference -Wpedantic -O3
CC_DBG	:= gcc -Wall -Wextra -Wformat=2 -Wnull-dereference -Wpedantic -g3

#make commands
all:	$(SRCS) $(HEADERS) $(MATHFNS) $(MISCFNS) $(STAKFNS)
		$(CC_ALL) $^ -o $(SRC)/claytor

debug:	$(SRCS) $(HEADERS) $(FUNCS)
		$(CC_DBG) $^ -o $(SRC)/claytor-debug

clean:
		rm -rf all
		rm -rf debug
