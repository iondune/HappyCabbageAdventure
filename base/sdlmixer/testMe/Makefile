LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
LIB=../../../lib/x64
else
LIB=../../../lib/x86
endif

#cabbage64:
#	gcc -g test.c `sdl-config --cflags --libs` -L../../../lib/x64 -Wl,-rpath=../../../lib/x64 -lSDL_mixer


cabbage:
	gcc -g test.c `sdl-config --cflags --libs` -L${LIB} -Wl,-rpath=${LIB} -lSDL_mixer
