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

CGameLevel::CGameLevel() : Loaded(false), Render(true) {

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

std::vector<CBiggerBlock*> & CGameLevel::getConsolidatedBlocks() {
   return blocksFinal;
}
