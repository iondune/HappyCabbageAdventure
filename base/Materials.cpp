#include "Materials.h"

struct MATERIAL {
   float Ka[4], Kd[4], Ks[4], Ke[4], Se;
};

struct MATERIAL m_Array[NUM_MATERIALS] = {
   //LIGHT_GREEN_MATTE
   {{0.5f, 0.7f, 0.5f, 1.0f},
      {0.6f, 0.8f, 0.6f, 1.0f},
      {0.1f, 0.3f, 0.1f, 1.0f},
      {0.0f, 0.0f, 0.0f, 0.0f}, 20},
   //DARK_GREEN_MATTE
   {{0.1f, 0.5f, 0.1f, 1.0f},
      {0.1f, 0.4f, 0.1f, 1.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
      {0.0f, 0.0f, 0.0f, 0.0f}, 10},
   //DARK_BROWN_MATTE
   {{0.8f, 0.52f, 0.25f, 1.0f},
      {0.8f, 0.52f, 0.25f, 1.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
      {0.0f, 0.0f, 0.0f, 0.0f}, 3}
};

void setMaterial(int num) {
   if(num > 0 && num <= NUM_MATERIALS) { //Indexed from 1
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_Array[num-1].Ka);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_Array[num-1].Kd);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_Array[num-1].Ks);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_Array[num-1].Ke);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_Array[num-1].Se);
   }
}
