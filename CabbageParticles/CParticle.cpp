#include "CParticle.h"

void CParticleRenderable::draw(CScene const * const scene) {
   printf("CParticleRenderable draw\n");
   if (! Visible)
      return;
   CRenderable::draw(scene);
}

CParticle::CParticle(SVector3 cPos) {
   centerPos = cPos;
}

void CParticle::setupRenderable() {
   renderable = new CParticleRenderable();
   CMesh * cube = CMeshLoader::createCubeMesh();
   cube->calculateNormalsPerFace();
   renderable->setMesh(cube);
   renderable->setTranslation(centerPos);
   renderable->getMaterial().Shader = CShaderLoader::loadShader("DiffuseTexture");
}

CMeshRenderable * CParticle::getRenderable() {
   return (CParticleRenderable*)renderable;
}
