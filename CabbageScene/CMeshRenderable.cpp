#include "CMeshRenderable.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"


CMeshRenderable::CMeshRenderable()
{}

CMesh * CMeshRenderable::getMesh()
{
    return Mesh;
}

void CMeshRenderable::setMesh(CMesh * mesh)
{
    removeAttribute("aPosition");
    removeAttribute("aColor");
    removeAttribute("aNormal");
    removeAttribute("aTexCoord");
    removeUniform("uTexColor");

    Mesh = mesh;

    addAttribute("aPosition", new CFloatVecAttribute(Mesh->makePositionBuffer(), 3));
    addAttribute("aColor", new CFloatVecAttribute(Mesh->makeColorBuffer(), 3));
    addAttribute("aNormal", new CFloatVecAttribute(Mesh->makeNormalBuffer(), 3));
    addAttribute("aNormalLine", new CFloatVecAttribute(Mesh->makeNormalLineBuffer(), 3));
    addAttribute("aTexCoord", new CFloatVecAttribute(Mesh->makeTexCoordBuffer(), 2));
    addUniform("uTexColor", new CIntUniform(0));

    setIndexBufferObject(Mesh->makeIndexBuffer());

    addRenderMode(GL_TEXTURE_2D);

    CRenderable * Normals = new CRenderable();
    Normals->addAttribute("aPosition", new CFloatVecAttribute(Mesh->makeNormalLineBuffer(), 3));
    Normals->addAttribute("aColor", new CFloatVecAttribute(Mesh->makeNormalColorBuffer(), 3));
    Normals->setIndexBufferObject(Mesh->makeNormalIndexBuffer());
    Normals->setShader(CShaderLoader::loadShader("Shaders/simple.vert", "Shaders/simple.frag"));
    Normals->setDrawType(GL_LINES);
    NormalObject = Normals;
}
