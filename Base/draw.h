#ifndef __DRAW_HEADER_
#define __DRAW_HEADER_
#include "../CabbageCore/SVector3.h"

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Flat, *Diffuse, *BlackShader, *DiffuseTexture, *normalColor, *Toon, *ToonTexture, *ToonBright;  //Use Diffuse for trees (doesn't need texture)

CImage *grassImg, *skyImg, *dirtImg, *blueFlwrImg, *pinkFlwrImg, *ficusImg,
       *poinImg, *flagImg;

CTexture *grassTxt, *skyTxt, *dirtTxt, *blueFlwrTxt, *pinkFlwrTxt, *ficusTxt,
         *poinTxt, *flagTxt;

CMesh *basicTreeMesh, *cabbageMesh, *christmasTreeMesh, *cubeMesh, *discMesh,
      *blueFlwrMesh, *pinkFlwrMesh, *ficusMesh, *poinMesh, *enemyMesh, 
      *flagMesh, *derpMesh;

CMeshSceneObject *renderShadow, *playerRenderable, *renderChristmasTree,
  *renderBasicTree, *renderBlueFlwr, *renderPinkFlwr, *renderFicus, *renderPoin,
  *tempRender, *renderFlag, *flagLogo, *renderDerp;

std::vector<CMeshSceneObject*> blocks, enemies, blueFlwrs, pinkFlwrs, ficuses,
  basicTrees, xmasTrees, poins;


void drawBlueFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   blueFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderBlueFlwr = new CMeshSceneObject();
   renderBlueFlwr->setMesh(blueFlwrMesh);
   renderBlueFlwr->setTexture(blueFlwrTxt);
   renderBlueFlwr->setShader(ToonTexture);
   renderBlueFlwr->setTranslation(SVector3(x, y, z));
   renderBlueFlwr->setScale(SVector3(scale));
   renderBlueFlwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderBlueFlwr);
}

void drawPinkFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   pinkFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderPinkFlwr = new CMeshSceneObject();
   renderPinkFlwr->setMesh(pinkFlwrMesh);
   renderPinkFlwr->setTexture(pinkFlwrTxt);
   renderPinkFlwr->setShader(ToonTexture);
   renderPinkFlwr->setTranslation(SVector3(x, y, z));
   renderPinkFlwr->setScale(SVector3(scale));
   renderPinkFlwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderPinkFlwr);
}

void drawPoin(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   poins.push_back(tempRender = new CMeshSceneObject());

   renderPoin = new CMeshSceneObject();
   renderPoin->setMesh(poinMesh);
   renderPoin->setTexture(poinTxt);
   renderPoin->setShader(ToonTexture);
   renderPoin->setTranslation(SVector3(x, y, z));
   renderPoin->setScale(SVector3(scale));
   renderPoin->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderPoin);
}

void drawFicus(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   ficuses.push_back(tempRender = new CMeshSceneObject());

   renderFicus = new CMeshSceneObject();
   renderFicus->setMesh(ficusMesh);
   //This is on purpose for now, will hopefully find a better texture later
   renderFicus->setTexture(blueFlwrTxt);
   renderFicus->setShader(ToonTexture);
   renderFicus->setTranslation(SVector3(x, y, z));
   renderFicus->setScale(SVector3(scale));
   renderFicus->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderFicus);
}

void drawBasicTree(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   basicTrees.push_back(tempRender = new CMeshSceneObject());

   renderBasicTree = new CMeshSceneObject();
   renderBasicTree->setMesh(basicTreeMesh);
   //renderBasicTree->getMaterial().Texture = (basicTreeTxt);
   renderBasicTree->setShader(Toon);
   renderBasicTree->setScale(SVector3(scale));
   renderBasicTree->setRotation(SVector3(-90, 0, 0));
   renderBasicTree->setTranslation(SVector3(x, y + scale/4.0f, z));

   app.getSceneManager().addSceneObject(renderBasicTree);
}

void drawChristmasTree(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   xmasTrees.push_back(tempRender = new CMeshSceneObject());

   renderChristmasTree = new CMeshSceneObject();
   renderChristmasTree->setMesh(christmasTreeMesh);
   //renderChristmasTree->getMaterial().Texture = (basicTreeTxt);
   renderChristmasTree->setShader(Toon);
   renderChristmasTree->setTranslation(SVector3(x, y, z));
   renderChristmasTree->setScale(SVector3(scale));
   renderChristmasTree->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderChristmasTree);
}
























/*//For Trees
obj_type basicTree;
obj_type christmasTree;

void drawDirt(int backwards) {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, dirtTexture);

   glBegin(GL_QUADS);
   if(!backwards) {
      glTexCoord2f(0, 4);
      glVertex3f(-25, 0, 2.5);
      glTexCoord2f(0, 0);
      glVertex3f(-25, -15, 2.5);
      glTexCoord2f(25, 0);
      glVertex3f(25, -15, 2.5);
      glTexCoord2f(25, 4);
      glVertex3f(25, 0, 2.5);
   }
   else {
      glTexCoord2f(0, 4);
      glVertex3f(-25, 0, -2.5);
      glTexCoord2f(0, 0);
      glVertex3f(-25, -15, -2.5);
      glTexCoord2f(25, 0);
      glVertex3f(25, -15, -2.5);
      glTexCoord2f(25, 4);
      glVertex3f(25, 0, -2.5);
   }
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

void drawSky(int backwards) {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, skyTexture);

   glBegin(GL_QUADS);
   if(!backwards) {
      glTexCoord2f(0, 1);
      glVertex3f(-25, 22, -2.5);
      glTexCoord2f(0, 0);
      glVertex3f(-25, -1, -2.5);
      glTexCoord2f(1, 0);
      glVertex3f(25, -1, -2.5);
      glTexCoord2f(1, 1);
      glVertex3f(25, 22, -2.5);
   }
   else {
      glTexCoord2f(0, 1);
      glVertex3f(-25, 20, 2.5);
      glTexCoord2f(0, 0);
      glVertex3f(-25, -1, 2.5);
      glTexCoord2f(1, 0);
      glVertex3f(25, -1, 2.5);
      glTexCoord2f(1, 1);
      glVertex3f(25, 20, 2.5);
   }
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

#include "Materials.h"
void drawTree(obj_type object, int material) {

   glPushMatrix();
   setMaterial(material);

   glRotatef(-90, 1, 0, 0);
   glScalef(2.0, 2.0, 2.0);

glBindTexture(GL_TEXTURE_2D, groundTexture);

//   glEnable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glBegin(GL_TRIANGLES);

    for (int l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //Calculate the normals for the tree
        //TODO: cache the normals so we don't calculate them every time a tree is rendered. this is a horrible performance hit as is :(
        SVector3 a = SVector3(
              object.vertex[ object.polygon[l_index].a ].x,
              object.vertex[ object.polygon[l_index].a ].y,
              object.vertex[ object.polygon[l_index].a ].z),
                 b = SVector3(
              object.vertex[ object.polygon[l_index].b ].x,
              object.vertex[ object.polygon[l_index].b ].y,
              object.vertex[ object.polygon[l_index].b ].z),
                 c = SVector3(
              object.vertex[ object.polygon[l_index].c ].x,
              object.vertex[ object.polygon[l_index].c ].y,
              object.vertex[ object.polygon[l_index].c ].z);
        SVector3 edge1 = a - b,
                 edge2 = a - c;
        SVector3 normal = edge1.crossProduct(edge2);
        //SVector3 normal = edge2.crossProduct(edge1);
        glNormal3f(normal.X, normal.Y, normal.Z);

        //----------------- FIRST VERTEX -----------------
        // Coordinates of the first vertex

        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);

        //----------------- THIRD VERTEX -----------------
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }

   glEnd();
   glDisable(GL_LIGHTING);
//glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

// draw an individual square
void drawSquare() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
        glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
}

//draw ground Plane for world
void drawPlane() {
    glPushMatrix();

    glEnable (GL_TEXTURE_2D);

    //glColor3f(0.3f, 0.7f, 0.7f);

    glBindTexture(GL_TEXTURE_2D, groundTexture);


    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex3f(-25, 0, -2.5);
    glTexCoord2f(0, 1);
    glVertex3f(-25, 0, 2.5);
    glTexCoord2f(50, 1);
    glVertex3f(25, 0, 2.5);
    glTexCoord2f(50, 0);
    glVertex3f(25, 0, -2.5);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawZBlock(float x, float y, float w, float h, float z) {
    glPushMatrix();
       glColor3f(0, 0, 0);
       //We want the center of the block
       glTranslatef((x+(x+w))/2, (y+(y+h))/2, z);
       glScalef(w, h, 1);
       //glutWireCube(1);
       glutSolidCube(1);
    glPopMatrix();
}

void drawBlock(float x, float y, float w, float h) {
    glPushMatrix();
       glColor3f(0, 0, 0);
       //We want the center of the block
       glTranslatef((x+(x+w))/2, (y+(y+h))/2, 0);
       glScalef(w, h, 1);
       //glutWireCube(1);
       glutSolidCube(1);
    glPopMatrix();
}*/
#endif
