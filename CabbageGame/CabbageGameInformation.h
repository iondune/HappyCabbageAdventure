#ifndef CABBAGE_GAME_INFORMATION_H
#define CABBAGE_GAME_INFORMATION_H

#include <set>

namespace Abilities {
   enum EAbilityType {
      SHIELD = 0,
      LASER = 1,
      DASH = 2,
      BLINK = 3,
      HEADBUTT = 4
   };
}

namespace Cabbage {
   //Information kept between levels
   class PlayerInformation {
      private:
         bool Godmode;

      public:
         int Health;
         int MaxHealth;
         int Energy;
         int MaxEnergy;
         int Lives;
         int Seeds;
         std::set<Abilities::EAbilityType> AvailableAbilities;
         bool canUseAbility(Abilities::EAbilityType type) {
            return Godmode || (AvailableAbilities.find(type) != AvailableAbilities.end());
         }

         void toggleGodmode() {
            Godmode = !Godmode;
         }

         bool isGodmodeEnabled() {
            return Godmode;
         }

         PlayerInformation() : Health(5), MaxHealth(5), Energy(100), MaxEnergy(100),
         Lives(3), Seeds(0), Godmode(false) {
            //Available abilities by default
            AvailableAbilities.insert(Abilities::LASER);
            AvailableAbilities.insert(Abilities::SHIELD);
            AvailableAbilities.insert(Abilities::HEADBUTT);
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
