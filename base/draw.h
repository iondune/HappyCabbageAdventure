//For Trees
obj_type object;

void drawTree() {

   glPushMatrix();

   glRotatef(-90, 1, 0, 0);
   glScalef(2.0, 2.0, 2.0);
   glBegin(GL_TRIANGLES);

    for (int l_index=0;l_index<object.polygons_qty;l_index++)
    {
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
    glColor3f(0.3f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
        glVertex3f(-25, 0, -2.5);
        glVertex3f(25, 0, -2.5);
        glVertex3f(25, 0, 2.5);
        glVertex3f(-25, 0, 2.5);
    glEnd();
    /*
    glBegin(GL_POLYGON);
        glVertex3f(-25, 0, 2.5);
        glVertex3f(25, 0, 2.5);
        glVertex3f(25, -2, 2.5);
        glVertex3f(-25, -2, 2.5);
    glEnd();
    */


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
    glEnd();
    glPopMatrix();
}

void drawBlock() {
    //Block->setArea(SRect2(-15, -1, 1, 5));
    glPushMatrix();
       glColor3f(0, 0, 0);
       glTranslatef(-14, 2, 0);
       glScalef(2, 1, 1);
       //glutWireCube(1);
       glutSolidCube(1);
    glPopMatrix();
}
