#ifndef _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_

#include <map>
#include <set>

#include "../CabbageCore/boost/shared_ptr.hpp"
#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"


class EDebugData
{

public:

    enum Domain
    {
        None = 0,
        Normals = 1,
        NormalColors = 2
    };

};

class CScene;

class IRenderable
{

protected:

    // Model Transformation
    SVector3 Translation, Rotation, Scale;
    SBoundingBox3 BoundingBox;

    int DebugDataFlags;

    bool Visible;

public:

    IRenderable();

    SVector3 const & getTranslation() const;
    SVector3 const & getRotation() const;
    SVector3 const & getScale() const;

    void setTranslation(SVector3 const & translation);
    void setRotation(SVector3 const & rotation);
    void setScale(SVector3 const & scale);

    void draw(CScene const * const scene);

    SBoundingBox3 const & getBoundingBox() const;
    SBoundingBox3 & getBoundingBox();
    void setBoundingBox(SBoundingBox3 const & boundingBox);

    bool const isDebugDataEnabled(EDebugData::Domain const type) const;
    void enableDebugData(EDebugData::Domain const type);
    void disableDebugData(EDebugData::Domain const type);

    bool const intersectsWithLine(SLine3 const & line) const;

    bool const isVisible() const;
    void setVisible(bool const isVisible);

};

#endif
