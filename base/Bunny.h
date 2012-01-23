#ifndef _BUNNY_H_INCLUDED_
#define _BUNNY_H_INCLUDED_

#include "CShader.h"
#include "Util/SVector3.h"
#include <GL/glut.h>

class Bunny
{

public:

  SVector3 position, direction;
  SVector3 AABBmin, AABBmax;
  bool stopped, hit;
  Bunny();
  Bunny(float px, float pz, float dx, float dz);
    ~Bunny();
    void step(float delta);
    void draw(CShader * Shader, GLuint PositionBufferHandle, GLuint ColorBufferHandle, int TriangleCount);
};

#endif
