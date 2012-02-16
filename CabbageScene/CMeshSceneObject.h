#ifndef _CABBAGE_SCENE_CMESHSCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CMESHSCENEOBJECT_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

#include "CSceneObject.h"


class CMeshSceneObject : public CSceneObject
{

protected:

    CMesh * Mesh;

public:

    CMeshSceneObject();

    CMesh * getMesh();
    void setMesh(CMesh * mesh);

};

#endif
