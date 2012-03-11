#include "CEngine.h"

class CBiggerBlock {
   public:
   float x, y, w, h;
   
   CBiggerBlock(float nx, float ny, float nw, float nh);
   
   void addToEngine(Cabbage::Collider::CEngine *Engine);
};

bool sortXY (CBiggerBlock* a, CBiggerBlock* b);
bool sortYX (CBiggerBlock* a, CBiggerBlock* b);
CBiggerBlock * consolidateY(CBiggerBlock *a, CBiggerBlock *b);
CBiggerBlock * consolidateX(CBiggerBlock *a, CBiggerBlock *b);
