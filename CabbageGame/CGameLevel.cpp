#include "CGameLevel.h"

#include "CGameplayElement.h"
#include "CElementBlock.h"
#include "CElementEnemy.h"

bool CGameLevel::isLoaded() {
   return Loaded;
}

void CGameLevel::setNoRender() {
   Render = false;
}

bool CGameLevel::shouldRender() {
   return Render;
}

CGameLevel::CGameLevel() : Loaded(false), Render(true), XmlCount(0) {

}

void CGameLevel::incrementXmlCount() {
   XmlCount++;
}

std::vector<CGameplayElement*> & CGameLevel::getElements() {
   return Elements;
}
std::vector<CElementBlock*> & CGameLevel::getBlocks() {
   return Blocks;
}

std::vector<CGroundBlock*> & CGameLevel::getGroundBlocks() {
   return GroundBlocks;
}

std::vector<CElementEnemy*> & CGameLevel::getEnemies() {
   return Enemies;
}

/*
std::vector<CFriend*> & CGameLevel::getFriends() {
   return Friends;
}
std::vector<CItem*> & CGameLevel::getItems() {
   return Items;
}
std::vector<CElevator*> & CGameLevel::getElevators() {
   return Elevators;
}
*/

bool CGameLevel::isNight() {
   return night;
}

void CGameLevel::toggleLoaded() {
   Loaded = !Loaded;
}

int CGameLevel::getEnvironment() {
   return env;
}

int CGameLevel::getEnv() {
   return env;
}

int CGameLevel::getXmlCount() {
   return XmlCount;
}

std::vector<CBiggerBlock*> & CGameLevel::getConsolidatedBlocks() {
   return blocksFinal;
}

void CGameLevel::printInformation() {
   std::cout << "There were " << XmlCount << " objects read from the xml file." << std::endl; 
   std::cout << "Of those, I was able to understand " << Elements.size() << " of them." << std::endl; 
   std::cout << "Here is what I understood: " << std::endl; 
   std::cout << "   This level takes place in environment " << env << (night?" at night.":" during the day.") << std::endl; 
   std::cout << "   Blocks: " << Blocks.size() << " (" << GroundBlocks.size() << " of them are ground blocks). The total was consolidated into " << blocksFinal.size() << " physics engine objects." << std::endl; 
   std::cout << "   Enemies: " << Enemies.size() << std::endl; 
}
