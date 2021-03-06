#ifndef __CBIGGERBLOCK_HEADER
#define __CBIGGERBLOCK_HEADER

class CCollisionEngine;

class CBiggerBlock {
   public:
   float x, y, w, h, z;
   
   CBiggerBlock(float nx, float ny, float nw, float nh, float);
   
   void addToEngine(CCollisionEngine *Engine);
};

bool sortXY (CBiggerBlock* a, CBiggerBlock* b);
bool sortYX (CBiggerBlock* a, CBiggerBlock* b);
CBiggerBlock * consolidateY(CBiggerBlock *a, CBiggerBlock *b);
CBiggerBlock * consolidateX(CBiggerBlock *a, CBiggerBlock *b);

#endif
