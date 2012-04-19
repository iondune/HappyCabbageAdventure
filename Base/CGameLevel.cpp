#include "CGameLevel.h"

std::vector<CPlaceable*> & CGameLevel::getPlaceables() {
   return Placeables;
}
std::vector<CPFriends*> & CGameLevel::getFriends() {
   return Friends;
}
std::vector<CPItem*> & CGameLevel::getItems() {
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
