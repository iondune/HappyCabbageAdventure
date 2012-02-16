#ifdef _CABBAGE_SCENE_CMESHRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CMESHRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

#include "CRenderable.h"


class CMeshRenderable : public CRenderable
{

protected:

    CMesh * Mesh;

    std::vector<CMeshRenderable *> SubRenderables;

public:

    CMeshRenderable();

    CMesh * getMesh();
    void setMesh(CMesh * mesh);

    void draw(CScene const * const scene);

};

#endif
