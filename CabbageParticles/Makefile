FILES = CParticle.cpp CParticleEngine.cpp CPLeaf.cpp CPCube.cpp CParticleObject.cpp CPFlame.cpp CPDeath.cpp CPLaser.cpp CPLaser2.cpp CPHurt.cpp CPBurst.cpp CPhysicsParticle.cpp CPDust.cpp
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I../Base

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageParticles.a

CParticle.o: CParticle.cpp CParticle.h
	$(CC) -o CParticle.o CParticle.cpp -c $(DIRECTORIES) $(OPENGL)

CPCube.o: CPCube.cpp CPCube.h
	$(CC) -o CPCube.o CPCube.cpp -c $(DIRECTORIES) $(OPENGL)

CPHurt.o: CPHurt.cpp CPHurt.h
	$(CC) -o CPHurt.o CPHurt.cpp -c $(DIRECTORIES) $(OPENGL)

CPLaser.o: CPLaser.cpp CPLaser.h
	$(CC) -o CPLaser.o CPLaser.cpp -c $(DIRECTORIES) $(OPENGL)

CPLaser2.o: CPLaser2.cpp CPLaser2.h
	$(CC) -o CPLaser2.o CPLaser2.cpp -c $(DIRECTORIES) $(OPENGL)

CParticleObject.o: CParticleObject.cpp CParticleObject.h
	$(CC) -I../CabbageScene/ -o CParticleObject.o CParticleObject.cpp -c $(DIRECTORIES) $(OPENGL)

CPLeaf.o: CPLeaf.cpp CPLeaf.h
	$(CC) -o CPLeaf.o CPLeaf.cpp -c $(DIRECTORIES) $(OPENGL)

CPFlame.o: CPFlame.cpp CPFlame.h
	$(CC) -o CPFlame.o CPFlame.cpp -c $(DIRECTORIES) $(OPENGL)

CPBurst.o: CPBurst.cpp CPBurst.h
	$(CC) -o CPBurst.o CPBurst.cpp -c $(DIRECTORIES) $(OPENGL)

CPDust.o: CPDust.cpp CPDust.h
	$(CC) -o CPDust.o CPDust.cpp -c $(DIRECTORIES) $(OPENGL)

CPhysicsParticle.o: CPhysicsParticle.cpp CPhysicsParticle.h
	$(CC) -o CPhysicsParticle.o CPhysicsParticle.cpp -c $(DIRECTORIES) $(OPENGL)

CPDeath.o: CPDeath.cpp CPDeath.h
	$(CC) -o CPDeath.o CPDeath.cpp -c $(DIRECTORIES) $(OPENGL)

CParticleEngine.o: CParticleEngine.cpp CParticleEngine.h CParticle.h CParticle.cpp
	$(CC) -o CParticleEngine.o CParticleEngine.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageParticles.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageParticles.a

