#ifndef _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"

#include "CCamera.h"

class IRenderable
{

protected:

    SVector3 Translation, Rotation, Scale;

    IRenderable();

public:

    SVector3 const & getTranslation() const;
    SVector3 const & getRotation() const;
    SVector3 const & getScale() const;

    void setTranslation(SVector3 const & translation);
    void setRotation(SVector3 const & rotation);
    void setScale(SVector3 const & scale);

    virtual void draw(CCamera const & Camera) =0;

};

#endif
