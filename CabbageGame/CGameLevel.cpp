#include "CGameLevel.h"

#include "CGameplayElement.h"
#include "CElementBlock.h"
#include "CElementBlockElevator.h"
#include "CElementEnemy.h"
#include "CElementPlayer.h"
#include "CElementItem.h"
#include "CElementBlockFlag.h"

bool CGameLevel::isLoaded() {
   return Loaded;
}

void CGameLevel::setNoRender() {
   Render = false;
}

bool CGameLevel::shouldRender() {
   return Render;
}

CGameLevel::CGameLevel() : Loaded(false), Render(true), XmlCount(0), PlayerElement(NULL) {

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

std::vector<CElementItem*> & CGameLevel::getItems() {
   return Items;
}

std::vector<CElementBlockFlag*> & CGameLevel::getFlags() {
   return Flags;
}

std::vector<CElementBlockElevator*> & CGameLevel::getElevators() {
   return Elevators;
}

/*
std::vector<CFriend*> & CGameLevel::getFriends() {
   return Friends;
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

CElementPlayer & CGameLevel::getPlayer() {
   return *PlayerElement;
}

void CGameLevel::printInformation() {
   std::cout << "There were " << XmlCount << " objects read from the xml file." << std::endl; 
   std::cout << "Of those, I was able to understand " << Elements.size() << " of them." << std::endl; 
   std::cout << "Here is what I understood: " << std::endl; 
   std::cout << "   This level takes place in environment " << env << (night?" at night.":" during the day.") << std::endl; 
   std::cout << "   The starting location is at [" << PlayerElement->getArea().Position.X << ", " << PlayerElement->getArea().Position.Y << "]." << std::endl; 
   std::cout << "   Blocks: " << Blocks.size() << " (" << GroundBlocks.size() << " of them are ground blocks). The total was consolidated into " << blocksFinal.size() << " physics engine objects." << std::endl; 
   std::cout << "   Enemies: " << Enemies.size() << std::endl; 
   std::cout << "   Items: " << Items.size() << std::endl; 
   std::cout << "   Flags: " << Flags.size() << std::endl; 
   for(int i = 0; i < Flags.size(); i++) {
      std::cout << "      Flag " << i << " is located at [" << Flags[i]->getArea().Position.X << ", " << Flags[i]->getArea().Position.Y << "], and has a role value of " << Flags[i]->getRole() << std::endl; 
   }
}

CCollisionEngine & CGameLevel::getPhysicsEngine() {
   return *PhysicsEngine;
}

void CGameLevel::setPhysicsEngine(CCollisionEngine *p) {
   PhysicsEngine = p;
}

void CGameLevel::removeEnemy(CElementEnemy *Enemy) {
   Enemies.erase(std::remove(Enemies.begin(), Enemies.end(), Enemy), Enemies.end());
}

void CGameLevel::addEnemy(CElementEnemy *Enemy) {
   Enemy->setupObjects();
   Enemies.push_back(Enemy);
   Elements.push_back(Enemy);
}

void CGameLevel::removeItem(CElementItem* Item) {
   Items.erase(std::remove(Items.begin(), Items.end(), Item), Items.end());
}

void CGameLevel::addItem(CElementItem* Item) {
   Item->setupObjects();
   Items.push_back(Item);
   Elements.push_back(Item);
}

void CGameLevel::removeObject(CGameplayElement* Object) {
   Elements.erase(std::remove(Elements.begin(), Elements.end(), Object), Elements.end());
   Blocks.erase(std::remove(Blocks.begin(), Blocks.end(), Object), Blocks.end());
   Enemies.erase(std::remove(Enemies.begin(), Enemies.end(), Object), Enemies.end());
   Items.erase(std::remove(Items.begin(), Items.end(), Object), Items.end());
   Flags.erase(std::remove(Flags.begin(), Flags.end(), Object), Flags.end());
   Elevators.erase(std::remove(Elevators.begin(), Elevators.end(), Object), Elevators.end());
}
