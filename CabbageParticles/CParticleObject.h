#ifndef _CABBAGE_SCENE_CPARTICLEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CPARTICLEOBJECT_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CabbageScene.h"

class CParticleObject : public CSceneObject
{
protected:
   CRenderable * particlesRenderable;
   CBufferObject<float> PositionBuffer, ColorBuffer, SizeBuffer;
   CBufferObject<unsigned short> IndexBuffer;
   SVector3 * positionsArr;
   bool updated;
   int numParticles;

public:
   CRenderable * getParticlesRenderable();
   void setup(SVector3 * vectorArr, SVector3 * colorArr, float * sizeArr, int num);
   void setPositions(SVector3 *vectorArr);
   void update();
};

#endif
