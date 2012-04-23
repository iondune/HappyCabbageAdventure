#include "CGameLevelLoader.h"

#include "CGameLevel.h"
#include "CElementBlock.h"
#include "CElementBlockFlag.h"
#include "CElementBlockElevator.h"
#include "CElementEnemy.h"
#include "CElementPlayer.h"
#include "CElementItem.h"
#include "CGameplayElement.h"

std::map<std::string, CGameLevel*> CGameLevelLoader::LoadedLevels;
CGameLevel *CGameLevelLoader::LatestLevel = NULL;
std::string CGameLevelLoader::LevelDirectory = "../Media/Levels/";
CGameLevel & CGameLevelLoader::getLatestLevel() {
   if(LatestLevel == NULL)
      LatestLevel = new CGameLevel();
   return *LatestLevel;
}

CGameLevel &CGameLevelLoader::loadLevel(std::string levelName) {
   return CGameLevelLoader::loadLevel(levelName, false);
}

CGameLevel &CGameLevelLoader::loadLevel(std::string levelName, bool useCache) {
	if (useCache)
	{
      //We aren't using cache currently
      /*
		std::map<std::string, CGameLevel *>::iterator it = LoadedLevels.find(levelName);

		if (it != LoadedLevels.end())
		{
			return *(it->second);
		}
      */
	}
   CGameLevel *newLevel = LatestLevel = new CGameLevel();
   std::vector<CBiggerBlock*> blocksY;
   int x,y,w,d,h,t, moving, env = -1;
   //float spd, rng;

   int count = 0;
   irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader((CGameLevelLoader::LevelDirectory + levelName).c_str());
   do {
   while (xml && xml->read())
   {
      switch(xml->getNodeType())
      {
      case irr::io::EXN_TEXT:
         break;
      case irr::io::EXN_ELEMENT:
         if(!strcmp("CBlock", xml->getNodeName()))
         {
            newLevel->incrementXmlCount();
            CElementBlock * ptr;
            // id, X, Y, height, width / from 0,1,2 so on
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            d = xml->getAttributeValueAsInt(4);
            t = xml->getAttributeValueAsInt(5);
            moving = xml->getAttributeValueAsInt(6);
            if (t == -5)
               newLevel->GroundBlocks.push_back(new CGroundBlock((float) x, (float) y, (float) w, (float) h, (float) d));

            if(xml->getAttributeValueAsInt(6)) {
               float range, speed;
               range = (float) xml->getAttributeValueAsInt(7);
               speed = (float) xml->getAttributeValueAsInt(8);
               CElementBlockElevator * ptr2;
               newLevel->Elevators.push_back(ptr2 = new CElementBlockElevator(SRect2((float)x, (float)y, (float)w, (float)h), d, t, range, speed));
               newLevel->Elements.push_back(ptr2);
            }
            else {
               newLevel->Blocks.push_back(ptr = new CElementBlock(SRect2((float)x, (float)y, (float)w, (float)h), d, t));
               newLevel->Elements.push_back(ptr);
               //ptr->isMovingPlatform = 0;
               //numBlocks++;
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
            newLevel->incrementXmlCount();
            //CEnemy *cen;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            CElementEnemy *myEnemy = CEnemyLoader::LoadEnemy(SRect2((float)x, (float)y, (float)w, (float)h), (Enemies::EEnemyType)t);
            if(myEnemy != NULL) {
               newLevel->Enemies.push_back(myEnemy);
               newLevel->Elements.push_back(myEnemy);
            }
            else
               printf("Not yet implemented: CElementEnemy type %d\n", t);
         }
         if(!strcmp("CCabbage",xml->getNodeName())) 
         {
            newLevel->incrementXmlCount();
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            CElementPlayer *player = new CElementPlayer(SRect2((float)x, (float)y, (float)h, (float)w));
            newLevel->Elements.push_back(player);
            newLevel->PlayerElement = player;
         }
         if(!strcmp("CFlag", xml->getNodeName()))
         {
            newLevel->incrementXmlCount();
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);

            CElementBlockFlag *myFlag = new CElementBlockFlag(SRect2((float)x, (float)y, (float)w, (float)h), t);
            newLevel->Flags.push_back(myFlag);
            newLevel->Elements.push_back(myFlag);
         }
         if(!strcmp("CPItem",xml->getNodeName()))
         {
            newLevel->incrementXmlCount();
            //CPItem * stuff;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            t = xml->getAttributeValueAsInt(2);
            CElementItem *myItem = CItemLoader::LoadItem(SRect2((float)x, (float)y, 1.0f, 1.0f), (Items::EItemType)t);
            if(myItem != NULL) {
               newLevel->Items.push_back(myItem);
               newLevel->Elements.push_back(myItem);
            }
            else
               printf("Not yet implemented: CElementItem type %d\n", t);
         }
         if(!strcmp("CPFriends",xml->getNodeName()))
         {
            newLevel->incrementXmlCount();
            //CPFriends * buds;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            t = xml->getAttributeValueAsInt(2);
            //TODO newLevel->Friends.push_back(buds = new CPFriends((float)x, (float)y,t));
            //TODO newLevel->Elements.push_back(buds);
            //buds->isMovingPlatform = 0;
            printf("Not yet implemented: CElementFriends type %d\n", t);
         }
         if (!strcmp("envVar", xml->getNodeName()))
         {
             newLevel->env = env = xml->getAttributeValueAsInt(0);
             newLevel->night = xml->getAttributeValueAsInt(1);
         }
         break;
      }
   }
   if(newLevel->getXmlCount() < 1) {
      xml = irr::io::createIrrXMLReader(levelName.c_str());
      count++;
   }
   else
      count = 10;
   } while(count < 2);
   if(newLevel->getXmlCount() < 1) {
      fprintf(stderr, "There was something wrong with the level file. %s could not be opened or had too few things.\n", (CGameLevelLoader::LevelDirectory + levelName).c_str()); 
      exit(1);
   }
   newLevel->blocksFinal = CGameLevelLoader::consolidateBlocks(blocksY);
	if (useCache)
		LoadedLevels[levelName] = newLevel;
   newLevel->toggleLoaded();
   return *newLevel;
}

std::vector<CBiggerBlock*> CGameLevelLoader::consolidateBlocks(std::vector<CBiggerBlock*> blocksY) {
   //printf("Size of blocksY: %d\n", blocksY.size());
   std::vector<CBiggerBlock*> blocksX, blocksFinal;

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
   //printf("Size of blocksX: %d\n", blocksX.size());
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

   //printf("Size of blocksFinal: %d\n", blocksFinal.size());
   /*
   for(unsigned int i = 0; i < blocksFinal.size(); i++) {
      //   printf("Block %d: [%0.0f,%0.0f,%0.0f,%0.0f]\n", i,
      //   blocksFinal[i]->x, blocksFinal[i]->y, blocksFinal[i]->w, blocksFinal[i]->h);
      blocksFinal[i]->addToEngine(Engine);
      delete blocksFinal[i];
      blocksFinal[i] = NULL;
   }
   */
   //printf("Total blocks saved: %d\n", numBlocks - blocksFinal.size());
   //numBlocks = blocksFinal.size();
   return blocksFinal;
}
