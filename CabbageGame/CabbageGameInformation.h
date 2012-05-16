#ifndef CABBAGE_GAME_INFORMATION_H
#define CABBAGE_GAME_INFORMATION_H

#include <set>

namespace Abilities {
   enum EAbilityType {
      SHIELD = 0,
      LASER = 1,
      DASH = 2,
      BLINK = 3,
      GODMODE = 100
   };
}

namespace Cabbage {
   //Information kept between levels
   class PlayerInformation {
      public:
         int Health;
         int MaxHealth;
         int Energy;
         int MaxEnergy;
         int Lives;
         int Seeds;
         std::set<Abilities::EAbilityType> AvailableAbilities;
         bool canUseAbility(Abilities::EAbilityType type) {
            return (AvailableAbilities.find(type) != AvailableAbilities.end());
         }

         PlayerInformation() : Health(5), MaxHealth(5), Energy(100), MaxEnergy(100),
         Lives(3), Seeds(0) {
            //Available abilities by default
            AvailableAbilities.insert(Abilities::LASER);
            AvailableAbilities.insert(Abilities::SHIELD);
         }
   };

   //Information kept in the Overworld
   class LevelInformation {
      public:
         int LevelsCompleted;
         int LastLevelEntered;

         LevelInformation() : LevelsCompleted(0), LastLevelEntered(0) {

         }
   };
}

#endif
