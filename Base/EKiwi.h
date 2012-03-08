/*
 * EKiwi.h
 *
 *  Created on: Feb 19, 2012
 *      Author: Klonoa
 */

#ifndef EKIWI_H_
#define EKIWI_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EKiwi : public CBadGuy
{

public:
   EKiwi(float x, float y, float w, float h, CGameplayManager* manager, int direction);

   float lastX;

   void update(const float TickTime);
   void loadMesh();
   void loadActor();
   void doRenderable();
   void DropBomb();

   float OrigX, SineValue, rotateBird;
   int inZ;
   int Direction;
   bool bombDropped;
};

#endif /* EKIWI_H_ */
