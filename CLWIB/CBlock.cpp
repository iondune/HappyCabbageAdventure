#include "CBlock.h"

CBlock::CBlock(float nx, float ny, int width, int height)
{
   x = nx; y = ny;
   w = width; h = height;
}

void CBlock::printXML() {
   //For usage in LWIB
   printf("X: %0.2f Y: %0.2f W: %d H: %d\n", x,y,w,h);
}

std::string CBlock::getX()
{
    std::stringstream xValue;
    xValue << x;
    return xValue.str();
}
std::string CBlock::getY()
{
    std::stringstream yValue;
    yValue << y;
    return yValue.str();
}
std::string CBlock::getWidth()
{
    std::stringstream widthValue;
    widthValue << w;
    return widthValue.str();
}
std::string CBlock::getHeight()
{
    std::stringstream heightValue;
    heightValue << h;
    return heightValue.str();
}
void CBlock::moveTo(float x,float y) {
   //For usage in LWIB
}
CMeshRenderable * CBlock::setupItem(CShader * shader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {
   Cabbage::Collider::CObject *engBlock = Engine->addObject();
   engBlock->setArea(SRect2(x, y, w, h));

   CMeshRenderable *tempBlock = new CMeshRenderable();
   CMesh *mesh = CMeshLoader::loadAsciiMesh("Cube");
   mesh->calculateNormalsPerFace();
   tempBlock->setMesh(mesh);
   tempBlock->getMaterial().Texture = CImageLoader::loadTexture("Textures/dirt.bmp");
   tempBlock->getMaterial().Shader = shader;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 1));
   tempBlock->setRotation(SVector3(0, 0, 0));
   CApplication::get().getSceneManager().addRenderable(tempBlock);

   return tempBlock;
}
