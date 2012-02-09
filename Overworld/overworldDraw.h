#ifndef __DRAW_HEADER_
#define __DRAW_HEADER_
#include "../CabbageCore/SVector3.h"

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Flat, *Diffuse, *DiffuseTexture, *normalColor;  //Use Diffuse for trees (doesn't need texture)

CImage *mapImg, *nodeImg, *discImg;

CTexture *mapTxt, *nodeTxt, *discTxt;

CMesh *basicTreeMesh, *cabbageMesh, *christmasTreeMesh, *enemyMesh,
      *mapMesh, *nodeMesh, *discMesh;

CMeshRenderable *playerRenderable, *renderChristmasTree,
  *renderBasicTree, *renderMap,  *renderNode, *tempRender;
#endif


void movePlayer(float x, float y) {
   
}

void drawNode(Node node) {
}

void DiscMesh() {
   discMesh = CMeshLoader::createDiscMesh();
   discMesh->linearizeIndices();
   discMesh->calculateNormalsPerFace();
}

