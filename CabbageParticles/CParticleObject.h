#ifndef _CABBAGE_SCENE_CPARTICLEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CPARTICLEOBJECT_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "../CabbageScene/CabbageScene.h"

class CParticleObject : public CSceneObject
{
protected:
   CRenderable * particlesRenderable;
   CBufferObject<float> PositionBuffer, ColorBuffer, SizeBuffer;
   CBufferObject<unsigned short> IndexBuffer;
   std::vector<SVector3*> positionsArr;
   bool updated;
   int numParticles;

public:
   CRenderable * getParticlesRenderable();
   void setup(std::vector<SVector3*> vectorArr, std::vector<SVector3*> colorArr, std::vector<float> sizeArr, int num);
   void setPositions(std::vector<SVector3*> vectorArr);
   void update();
   void draw(CScene const * const scene);
};

#endif
