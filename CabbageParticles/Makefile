FILES = CParticle.cpp CParticleEngine.cpp 

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageParticles.a

CParticle.o: CParticle.cpp CParticle.h
	g++ -o CParticle.o CParticle.cpp -c $(DIRECTORIES) $(OPENGL)

CParticleEngine.o: CParticleEngine.cpp CParticleEngine.h CParticle.h CParticle.cpp
	g++ -o CParticleEngine.o CParticleEngine.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageParticles.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageParticles.a

