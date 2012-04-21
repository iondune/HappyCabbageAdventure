#include "CGameLevel.h"

#include "CGameplayElement.h"
#include "CElementBlock.h"

bool CGameLevel::isLoaded() {
   return Loaded;
}

CGameLevel::CGameLevel() : Loaded(false) {

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

/*
std::vector<CFriend*> & CGameLevel::getFriends() {
   return Friends;
}
std::vector<CItem*> & CGameLevel::getItems() {
   return Items;
}
std::vector<CEnemy*> & CGameLevel::getEnemies() {
   return Enemies;
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
