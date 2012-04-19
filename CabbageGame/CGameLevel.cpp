#include "CGameLevel.h"

std::vector<CGameplayElement*> & CGameLevel::getElements() {
   return Elements;
}
std::vector<CFriend*> & CGameLevel::getFriends() {
   return Friends;
}
std::vector<CItem*> & CGameLevel::getItems() {
   return Items;
}
std::vector<CBlock*> & CGameLevel::getBlocks() {
   return Blocks;
}
std::vector<CEnemy*> & CGameLevel::getEnemies() {
   return Enemies;
}
std::vector<CGroundBlock*> & CGameLevel::getGroundBlocks() {
   return GroundBlocks;
}
std::vector<CElevator*> & CGameLevel::getElevators() {
   return Elevators;
}

bool CGameLevel::isNight() {
   return night;
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
