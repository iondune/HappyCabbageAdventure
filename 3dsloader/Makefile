CC			= g++
OBJ			= tutorial4.o init.o texture.o 3dsloader.o

TARGET		= tutorial4

$(TARGET):	$(OBJ)
	$(CC)  -o $(TARGET) $(OBJ) `sdl-config --libs` -lGL -lGLU -lm

tutorial4.o:		tutorial4.cpp 
	$(CC) -c tutorial4.cpp `sdl-config --cflags`

init.o:			init.cpp 
	$(CC) -c init.cpp `sdl-config --cflags`

texture.o:		texture.cpp 
	$(CC) -c texture.cpp `sdl-config --cflags`

3dsloader.o:		3dsloader.cpp 
	$(CC) -c 3dsloader.cpp `sdl-config --cflags`

clean:
	rm -f $(TARGET) $(OBJ)
