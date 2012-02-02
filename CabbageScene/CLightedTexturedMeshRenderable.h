#ifndef _CABBAGE_SCENE_CLIGHTEDTEXTUREDRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CLIGHTEDTEXTUREDRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

#include "CRenderable.h"


class CLightedTexturedMeshRenderable : public CRenderable
{

protected:

    GLuint PositionBufferHandle;
    GLuint ColorBufferHandle;
    GLuint IndexBufferHandle;
    GLuint TexCoordBufferHandle;
    GLuint NormalBufferHandle;

    GLsizei IndexCount;

    CShader * Shader;
    CTexture * Texture;

public:

    CLightedTexturedMeshRenderable(CMesh const & Mesh);

    CShader * getShader();
    CTexture * getTexture();

    void setShader(CShader * shader);
    void setTexture(CTexture * texture);

    virtual void draw(CCamera const & Camera);

};

#endif
