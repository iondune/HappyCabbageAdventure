#ifndef _CABBAGE_SCENE_CTEXTUREDRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CTEXTUREDRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

#include "IRenderable.h"


class CMeshRenderable : public IRenderable
{

protected:

    GLuint PositionBufferHandle;
    GLuint ColorBufferHandle;
    GLuint IndexBufferHandle;

    GLsizei IndexCount;

    CShader * Shader;

public:

    CMeshRenderable(CMesh const & Mesh);

    CShader * getShader();

    void setShader(CShader * shader);

    virtual void draw(CCamera const & Camera);

};

#endif
