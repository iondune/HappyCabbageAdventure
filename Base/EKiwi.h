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

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   float OrigX;
   int Direction;
};

#endif /* EKIWI_H_ */
