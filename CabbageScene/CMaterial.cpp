#include "CMaterial.h"

CMaterial::CMaterial()
    : Texture(0), BindAmbientColor(AmbientColor), BindDiffuseColor(DiffuseColor), BindShininess(Shininess)
{
	loadDefaults();
}

void CMaterial::loadDefaults()
{
	Shininess = 200.f;
	AmbientColor = SColor(0.15f, 0.15f, 0.15f);
	DiffuseColor = SColor(0.8f, 0.8f, 0.8f);
}
