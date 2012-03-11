#include "CBiggerBlock.h"

CBiggerBlock::CBiggerBlock(float nx, float ny, float nw, float nh) : x(nx), y(ny), w(nw), h(nh) {
}

void CBiggerBlock::addToEngine(Cabbage::Collider::CEngine *Engine) {
   Cabbage::Collider::CObject *block = Engine->addObject();
   block->setArea(SRect2(x, y, w, h));
}

bool sortXY (CBiggerBlock* a, CBiggerBlock* b) {
   if(a->x == b->x) {
      return a->y >= b->y;
   }
   else return a->x >= b->x;
}

bool sortYX (CBiggerBlock* a, CBiggerBlock* b) {
   if(a->y == b->y) {
      return a->x >= b->x;
   }
   else return a->y >= b->y;
}

CBiggerBlock * consolidateY(CBiggerBlock *a, CBiggerBlock *b) {
   if(b->x == a->x && b->w == a->w) {
      // a is on the bottom
      if((b->y == (a->y + a->h))) {
         return new CBiggerBlock(a->x, a->y, a->w, a->h + b->h);
      }
      // b is on the bottom
      else if(a->y == (b->y + b->h)) {
         return new CBiggerBlock(b->x, b->y, a->w, a->h + b->h);
      }
   }
   return NULL;
}

CBiggerBlock * consolidateX(CBiggerBlock *a, CBiggerBlock *b) {
   if(b->y == a->y && b->h == a->h) {
      // a is on the left
      if((b->x == (a->x + a->w))) {
         return new CBiggerBlock(a->x, a->y, a->w + b->w, a->h);
      }
      // b is on the right
      else if((a->x == (b->x + b->w))) {
         return new CBiggerBlock(b->x, b->y, a->w + b->w, a->h);
      }
   }
   return NULL;
}
