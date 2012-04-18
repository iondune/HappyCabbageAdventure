#include "CGameLevelLoader.h"

CGameLevelLoader::CGameLevelLoader(const char* levelName) {
   //Block consolidation algorithm code
   numBlocks = 0;
   blocksY.clear();

   int x,y,w,d,h,t, moving;
   //float spd, rng;

   irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(levelName);
   while (xml && xml->read())
   {
      switch(xml->getNodeType())
      {
      case irr::io::EXN_TEXT:
         break;
      case irr::io::EXN_ELEMENT:
         if(!strcmp("CBlock", xml->getNodeName()))
         {
            CBlock * ptr;
            // id, X, Y, height, width / from 0,1,2 so on
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            d = xml->getAttributeValueAsInt(4);
            t = xml->getAttributeValueAsInt(5);
            moving = xml->getAttributeValueAsInt(6);
            if (t == -5)
               GroundBlocks.push_back(new CGroundBlock((float) x, (float) y, (float) w, (float) h, (float) d));

            Blocks.push_back(ptr = new CBlock((float)x,(float)y,w,h,d,t,moving, env));
            Placeables.push_back(ptr);
            if(xml->getAttributeValueAsInt(6)) {
               ptr->isMovingPlatform = 1;
               ptr->Range = (int) xml->getAttributeValueAsFloat(7); //Range
               ptr->Speed = (int) xml->getAttributeValueAsFloat(8); //Speed
            }
            else {
               ptr->isMovingPlatform = 0;
               numBlocks++;
               int curW = w;
               //Hard case for ground blocks...
               if(w == 5 && h == 5 && d == 5) {
                  blocksY.push_back(new CBiggerBlock((float)x, (float)y, (float)w, (float)h, (float)d));
               }
               else
                  for(; curW > 0; curW--)
                     blocksY.push_back(new CBiggerBlock((float)x + (w - curW), (float)y, 1.0f, (float)h, (float)d));
            }
         }
         if(!strcmp("CEnemy", xml->getNodeName()))
         {
            CEnemy *cen;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            Enemies.push_back(cen = new CEnemy((float)x,(float)y,w,h,t, env));
            Placeables.push_back(cen);
            cen->isMovingPlatform = 0;
         }
         if(!strcmp("CCabbage",xml->getNodeName())) {
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            //TODO: Change this
            //Player->setArea(SRect2((float)x, (float)y, (float)h, (float)w));
         }
         if(!strcmp("CFlag", xml->getNodeName())) {
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            /* TODO: Change this (make it use Flag placeables and add it as a CGameplayElement)
            if (t == 0) {
               victoryBlock = Engine->addObject();
               victoryBlock->setArea(SRect2((float)x,(float)y, 1, 5));
               flagLogo->setTranslation(SVector3((float)x, (float) y+.9f, 1.0f));
               renderFlag->setTranslation(SVector3((float)x,(float) y+.5f, 1.0f));
               GameplayManager->setVictoryFlag(victoryBlock);
            }
            if (t == 1){
               secretVictoryBlock = Engine->addObject();
               secretVictoryBlock->setArea(SRect2((float)x,(float)y, 1, 5));

               flagLogo2->setTranslation(SVector3((float)x, (float) y+.9f, 1.0f));
               renderSecretFlag->setTranslation(SVector3((float)x,(float) y+.5f, 1.0f));
               GameplayManager->setSecretVictoryFlag(secretVictoryBlock);
               //add victory flag
            }
            */
         }
         if(!strcmp("CPItem",xml->getNodeName())) {
            CPItem * stuff;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            t = xml->getAttributeValueAsInt(2);
            Items.push_back(stuff = new CPItem((float)x,(float)y,t));
            Placeables.push_back(stuff);
            stuff->isMovingPlatform = 0;
         }
         if(!strcmp("CPFriends",xml->getNodeName())) {
            CPFriends * buds;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            t = xml->getAttributeValueAsInt(2);
            Friends.push_back(buds = new CPFriends((float)x, (float)y,t));
            Placeables.push_back(buds);
            buds->isMovingPlatform = 0;
         }
         if (!strcmp("envVar", xml->getNodeName()))
         {
             env = xml->getAttributeValueAsInt(0);
             night = xml->getAttributeValueAsInt(1);
         }
         break;
      }
   }
}

std::vector<CPlaceable*> & CGameLevelLoader::getPlaceables() {
   return Placeables;
}
std::vector<CPFriends*> & CGameLevelLoader::getFriends() {
   return Friends;
}
std::vector<CPItem*> & CGameLevelLoader::getItems() {
   return Items;
}
std::vector<CBlock*> & CGameLevelLoader::getBlocks() {
   return Blocks;
}
std::vector<CEnemy*> & CGameLevelLoader::getEnemies() {
   return Enemies;
}
std::vector<CGroundBlock*> & CGameLevelLoader::getGroundBlocks() {
   return GroundBlocks;
}

bool CGameLevelLoader::isNight() {
   return night;
}

int CGameLevelLoader::getEnv() {
   return env;
}

void CGameLevelLoader::consolidateAndAddBlocks() {
   printf("Size of blocksY: %d\n", blocksY.size());
   std::vector<CBiggerBlock*> blocksX;

   sort(blocksY.begin(), blocksY.end(), sortXY);

   CBiggerBlock *curBlock = blocksY[0];
   for(unsigned int i = 1; i < blocksY.size(); i++) {
      CBiggerBlock * newBlock = consolidateY(curBlock, blocksY[i]);
      /*
         printf("Consolidation of [%0.0f,%0.0f,%0.0f,%0.0f] and [%0.0f,%0.0f,%0.0f,%0.0f]? %s\n", 
         curBlock->x, curBlock->y, curBlock->w, curBlock->h,
         blocksY[i]->x, blocksY[i]->y, blocksY[i]->w, blocksY[i]->h,
         newBlock==NULL?"NO":"YES");
         */
      // There was nothing to consolidate, which means this CBiggerBlock is done (in the Y direction)
      if(newBlock == NULL) {
         blocksX.push_back(curBlock);
         curBlock = blocksY[i];
      }
      else {
         delete curBlock;
         delete blocksY[i];
         blocksY[i] = NULL;
         curBlock = newBlock;
      }
   }
   blocksX.push_back(curBlock);

   sort(blocksX.begin(), blocksX.end(), sortYX);
   printf("Size of blocksX: %d\n", blocksX.size());
   curBlock = blocksX[0];
   for(unsigned int i = 1; i < blocksX.size(); i++) {
      CBiggerBlock * newBlock = consolidateX(curBlock, blocksX[i]);
      /*
         printf("Consolidation of [%0.0f,%0.0f,%0.0f,%0.0f] and [%0.0f,%0.0f,%0.0f,%0.0f]? %s\n", 
         curBlock->x, curBlock->y, curBlock->w, curBlock->h,
         blocksX[i]->x, blocksX[i]->y, blocksX[i]->w, blocksX[i]->h,
         newBlock==NULL?"NO":"YES");
         */

      // There was nothing to consolidate, which means this CBiggerBlock is done (in the X direction)
      if(newBlock == NULL) {
         blocksFinal.push_back(curBlock);
         curBlock = blocksX[i];
      }
      else {
         delete curBlock;
         delete blocksX[i];
         blocksX[i] = NULL;
         curBlock = newBlock;
      }
   }
   blocksFinal.push_back(curBlock);

   printf("Size of blocksFinal: %d\n", blocksFinal.size());
   /*
   for(unsigned int i = 0; i < blocksFinal.size(); i++) {
      //   printf("Block %d: [%0.0f,%0.0f,%0.0f,%0.0f]\n", i,
      //   blocksFinal[i]->x, blocksFinal[i]->y, blocksFinal[i]->w, blocksFinal[i]->h);
      blocksFinal[i]->addToEngine(Engine);
      delete blocksFinal[i];
      blocksFinal[i] = NULL;
   }
   */
   printf("Total blocks saved: %d\n", numBlocks - blocksFinal.size());
   numBlocks = blocksFinal.size();
   blocksX.clear();
   blocksY.clear();
}

std::vector<CBiggerBlock*> & CGameLevelLoader::getConsolidatedBlocks() {
   return blocksFinal;
}
