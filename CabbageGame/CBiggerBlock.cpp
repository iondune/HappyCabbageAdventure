#include "CBiggerBlock.h"
#include "CElementBlock.h"

#define MAX(x,y) ((x)>(y)?(x):(y))
CBiggerBlock::CBiggerBlock(float nx, float ny, float nw, float nh, float d) : x(nx), y(ny), w(nw), h(nh), z(d) {
}

void CBiggerBlock::addToEngine(CCollisionEngine *Engine) {
   CCollisionObject *block = Engine->addObject();
   block->setCollisionResponder(NULL);
   block->setArea(SRect2f(x, y, w, h));
   block->setDepth(z);
   block->setElement(new CElementBlock(SRect2f(x, y, w, h), (int) z, 3));
}

bool sortXY (CBiggerBlock* a, CBiggerBlock* b) {
   if(a->x == b->x) {
      return a->y < b->y;
   }
   else return a->x < b->x;
}

bool sortYX (CBiggerBlock* a, CBiggerBlock* b) {
   if(a->y == b->y) {
      return a->x < b->x;
   }
   else return a->y < b->y;
}

CBiggerBlock * consolidateY(CBiggerBlock *a, CBiggerBlock *b) {
   if(b->x == a->x && b->w == a->w) {
      // a is on the bottom
      if((b->y == (a->y + a->h))) {
         return new CBiggerBlock(a->x, a->y, a->w, a->h + b->h, MAX(a->z, b->z));
      }
      // b is on the bottom
      else if(a->y == (b->y + b->h)) {
         return new CBiggerBlock(b->x, b->y, a->w, a->h + b->h, MAX(a->z, b->z));
      }
   }
   return NULL;
}

CBiggerBlock * consolidateX(CBiggerBlock *a, CBiggerBlock *b) {
   if(b->y == a->y && b->h == a->h) {
      // a is on the left
      if((b->x == (a->x + a->w))) {
         return new CBiggerBlock(a->x, a->y, a->w + b->w, a->h, MAX(a->z, b->z));
      }
      // b is on the right
      else if((a->x == (b->x + b->w))) {
         return new CBiggerBlock(b->x, b->y, a->w + b->w, a->h, MAX(a->z, b->z));
      }
   }
   return NULL;
}
