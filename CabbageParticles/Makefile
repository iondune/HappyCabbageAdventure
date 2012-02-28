FILES = CParticle.cpp CParticleEngine.cpp CPLeaf.cpp CPCube.cpp CParticleObject.cpp CPFlame.cpp CPDeath.cpp CPLaser.cpp 

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageParticles.a

CParticle.o: CParticle.cpp CParticle.h
	g++ -O1 -o CParticle.o CParticle.cpp -c $(DIRECTORIES) $(OPENGL)

CPCube.o: CPCube.cpp CPCube.h
	g++ -O1 -o CPCube.o CPCube.cpp -c $(DIRECTORIES) $(OPENGL)

CPLaser.o: CPLaser.cpp CPLaser.h
	g++ -O1 -o CPLaser.o CPLaser.cpp -c $(DIRECTORIES) $(OPENGL)

CParticleObject.o: CParticleObject.cpp CParticleObject.h
	g++ -O1 -I../CabbageScene/ -o CParticleObject.o CParticleObject.cpp -c $(DIRECTORIES) $(OPENGL)

CPLeaf.o: CPLeaf.cpp CPLeaf.h
	g++ -O1 -o CPLeaf.o CPLeaf.cpp -c $(DIRECTORIES) $(OPENGL)

CPFlame.o: CPFlame.cpp CPFlame.h
	g++ -O1 -o CPFlame.o CPFlame.cpp -c $(DIRECTORIES) $(OPENGL)

CPDeath.o: CPDeath.cpp CPDeath.h
	g++ -O1 -o CPDeath.o CPDeath.cpp -c $(DIRECTORIES) $(OPENGL)


CParticleEngine.o: CParticleEngine.cpp CParticleEngine.h CParticle.h CParticle.cpp
	g++ -O1 -o CParticleEngine.o CParticleEngine.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageParticles.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageParticles.a

