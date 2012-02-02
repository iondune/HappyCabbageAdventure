#ifndef _CABBAGE_SCENE_CMESHRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CMESHRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

#include "CRenderable.h"


class CMeshRenderable : public CRenderable
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
