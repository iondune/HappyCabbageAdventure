LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
LIB=-L../../../lib/x64
else
LIB=-L../../../lib/x86
endif
TEST=-L../../../lib

#cabbage64:
#	gcc -g test.c `sdl-config --cflags --libs` -L../../../lib/x64 -Wl,-rpath=../../../lib/x64 -lSDL_mixer


cabbage:
	gcc -g test.c `sdl-config --cflags --libs` ${LIB} ${TEST} -Wl,-rpath=${LIB} -lSDL_mixer
