UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
LIB=-lSDL -lSDL_image -lm
else
LIB=-lmingw32 -lSDLmain -lSDL -lSDL_image -lm
endif

all:
	gcc *.c $(LIB) -o jam -g
