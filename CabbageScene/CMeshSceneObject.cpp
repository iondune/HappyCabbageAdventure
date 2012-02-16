#include "CMeshSceneObject.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"


CMeshSceneObject::CMeshSceneObject()
{}

CMesh * CMeshSceneObject::getMesh()
{
    return Mesh;
}

void CMeshSceneObject::setMesh(CMesh * mesh)
{
    Mesh = mesh;

    if (Mesh)
    {
        for (unsigned int i = 0; i < Mesh->PositionBuffers.size(); ++ i)
        {
            CRenderable * Child = 0;
            if (Renderables.size() > i)
                Child = Renderables[i];
            else
            {
                Renderables.push_back(Child = new CRenderable());
            }

            // Remove any attributes which might have been set by a previous mesh
            Child->removeAttribute("aPosition");
            Child->removeAttribute("aColor");
            Child->removeAttribute("aNormal");
            Child->removeAttribute("aTexCoord");
            Child->removeUniform("uTexColor");

            // Add mesh attributes
            Child->addAttribute("aPosition", new SAttribute<float>(Mesh->PositionBuffers[i], 3));
            Child->addAttribute("aColor", new SAttribute<float>(Mesh->ColorBuffers[i], 3));
            Child->addAttribute("aNormal", new SAttribute<float>(Mesh->NormalBuffers[i], 3));
            Child->addAttribute("aTexCoord", new SAttribute<float>(Mesh->TexCoordBuffers[i], 2));
            Child->addUniform("uTexColor", new SUniform<int>(0));

            Child->getMaterial().DiffuseColor = Mesh->MeshBuffers[i]->DiffuseColor;

            // Add mesh index buffer
            Child->setIndexBufferObject(Mesh->IndexBuffers[i]);

            // Set bounding box
            //Child->BoundingBox = Mesh->getBoundingBox();

            // Remove any previous normal debugging object
            if (Child->getDebuggingNormalObject())
            {
                delete Child->getDebuggingNormalObject();
                Child->getDebuggingNormalObject() = 0;
            }

            // Add normal debugging object
            Child->getDebuggingNormalObject() = new CRenderable();
            Child->getDebuggingNormalObject()->addAttribute("aPosition", new SAttribute<float>(Mesh->NormalLineBuffers[i], 3));
            Child->getDebuggingNormalObject()->addAttribute("aColor", new SAttribute<float>(Mesh->NormalColorBuffers[i], 3));
            Child->getDebuggingNormalObject()->setIndexBufferObject(Mesh->NormalIndexBuffers[i]);
            Child->getDebuggingNormalObject()->setShader(CShaderLoader::loadShader("Simple"));
            Child->getDebuggingNormalObject()->setDrawType(GL_LINES);

            // Reset the shader to load attributes again
            Child->reloadVariablesOnNextDraw();
        }
    }

}
