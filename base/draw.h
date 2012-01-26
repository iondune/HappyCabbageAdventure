#include "../CabbageCore/SVector3.h"
//For Trees
obj_type object;

void drawDirt() {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   //glColor3f(.04, .3, .43);

   glBindTexture(GL_TEXTURE_2D, dirtTexture);

   glBegin(GL_QUADS);
   glTexCoord2f(0, 4);
   glVertex3f(-25, 0, 2.5);
   glTexCoord2f(0, 0);
   glVertex3f(-25, -15, 2.5);
   glTexCoord2f(25, 0);
   glVertex3f(25, -15, 2.5);
   glTexCoord2f(25, 4);
   glVertex3f(25, 0, 2.5);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

void drawSky() {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   //glColor3f(.04, .3, .43);

   glBindTexture(GL_TEXTURE_2D, skyTexture);

   glBegin(GL_QUADS);
   glTexCoord2f(0, 1);
   glVertex3f(-25, 15, -2.5);
   glTexCoord2f(0, 0);
   glVertex3f(-25, -1, -2.5);
   glTexCoord2f(1, 0);
   glVertex3f(25, -1, -2.5);
   glTexCoord2f(1, 1);
   glVertex3f(25, 15, -2.5);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

#include "Materials.h"
void drawTree() {

   glPushMatrix();
   setMaterial(DARK_GREEN_MATTE);

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

/*
    glTexCoord2f(0, 0);
    glVertex3f(-25, 0, -2.5);

    glTexCoord2f(1, 0);
    glVertex3f(25, 0, -2.5);

    glTexCoord2f(1, 1);
    glVertex3f(25, 0, 2.5);

    glTexCoord2f(0, 1);
    glVertex3f(-25, 0, 2.5);*/

    glEnd();
/*    glBegin(GL_POLYGON);
        glVertex3f(-25, 0, -2.5);
        glVertex3f(25, 0, -2.5);
        glVertex3f(25, 0, 2.5);
        glVertex3f(-25, 0, 2.5);
    glEnd();

    glColor3f(0, 0, 0);
    glPointSize(15.f);
    glBegin(GL_LINES);
    for(float i = -25; i < 25; i += 0.5) {
       glVertex3f(i, 0.05, -2.5);
       glVertex3f(i, 0.05, 2.5);
       if(i <= 2.5 && i >= -2.5) {
          glVertex3f(-25, 0.05, i);
          glVertex3f(25, 0.05, i);
       }
    }
    glEnd();*/

    glDisable(GL_TEXTURE_2D);
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
}
