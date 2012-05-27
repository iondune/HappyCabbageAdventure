#include "CMaterial.h"

CMaterial::CMaterial()
{
	loadDefaults();
}

void CMaterial::loadDefaults()
{
	Shininess = 100000000000.0f;
	AmbientColor = SColor(0.15f, 0.15f, 0.15f);
	DiffuseColor = SColor(0.8f, 0.8f, 0.8f);
}
