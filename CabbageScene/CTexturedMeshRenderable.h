#ifndef _CABBAGE_SCENE_CTEXTUREDRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CTEXTUREDRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

#include "IRenderable.h"


class CTexturedMeshRenderable : public IRenderable
{

protected:

    GLuint PositionBufferHandle;
    GLuint ColorBufferHandle;
    GLuint IndexBufferHandle;
    GLuint TexCoordBufferHandle;

    GLsizei IndexCount;

    CShader * Shader;
    CTexture * Texture;

public:

    CTexturedMeshRenderable(CMesh const & Mesh);

    CShader * getShader();
    CTexture * getTexture();

    void setShader(CShader * shader);
    void setTexture(CTexture * texture);

    virtual void draw();

};

#endif
