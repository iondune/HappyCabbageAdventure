FILES = CParticle.cpp CParticleEngine.cpp CPLeaf.cpp CPCube.cpp CParticleObject.cpp CPFlame.cpp CPDeath.cpp CPLaser.cpp CPLaser2.cpp CPHurt.cpp CPBurst.cpp CPhysicsParticle.cpp CPDust.cpp CPStar.cpp CPLaserCharged.cpp
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I../Base

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

all: ../lib/libCabbageParticles.a

../lib/libCabbageParticles.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageParticles.a

