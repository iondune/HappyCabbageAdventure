#ifndef EAPPLE_H_
#define EAPPLE_H_

class EApple : public CBadGuy {

   void loadMesh();
   bool loadActor();

public:
   EApple(float x, float y, float w, float h, CGameplayManager* manager);


};

#endif /* EAPPLE_H_ */
