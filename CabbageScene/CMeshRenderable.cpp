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
    // Remove any attributes which might have been set by a previous mesh
    removeAttribute("aPosition");
    removeAttribute("aColor");
    removeAttribute("aNormal");
    removeAttribute("aTexCoord");
    removeUniform("uTexColor");

    Mesh = mesh;

    // Remove any previous normal debugging object
    if (NormalObject)
    {
        delete NormalObject;
        NormalObject = 0;
    }

    if (Mesh)
    {
        // Add mesh attributes
        addAttribute("aPosition", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(Mesh->makePositionBuffer(), 3)));
        addAttribute("aColor", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(Mesh->makeColorBuffer(), 3)));
        addAttribute("aNormal", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(Mesh->makeNormalBuffer(), 3)));
        addAttribute("aTexCoord", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(Mesh->makeTexCoordBuffer(), 2)));
        addUniform("uTexColor", boost::shared_ptr<IUniform>(new CIntUniform(0)));

        // Add mesh index buffer
        setIndexBufferObject(Mesh->makeIndexBuffer());

        // Set bounding box
        BoundingBox = Mesh->getBoundingBox();

        // Add normal debugging object
        NormalObject = new CRenderable();
        NormalObject->addAttribute("aPosition", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(Mesh->makeNormalLineBuffer(), 3)));
        NormalObject->addAttribute("aColor", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(Mesh->makeNormalColorBuffer(), 3)));
        NormalObject->setIndexBufferObject(Mesh->makeNormalIndexBuffer());
        Material.Shader = CShaderLoader::loadShader("Simple");
        NormalObject->setDrawType(GL_LINES);

        // Reset the shader to load attributes again
        LastLoadedShader = 0;
    }

}
