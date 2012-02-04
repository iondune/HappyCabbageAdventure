#ifndef __SOUND_INCLUDE_H__
#define  __SOUND_INCLUDE_H__

#include "SDL_mixer.h"

//Used to run current soundtrack
extern Mix_Music *music;

//Sound Effects
extern Mix_Chunk *die;
extern Mix_Chunk *jump;
extern Mix_Chunk *takeDmg;
extern Mix_Chunk *killEnemy;

//Misc Sound Variables
extern bool playDead;
extern bool playTakeDmg;
extern bool playJump;
extern bool musicOn;

void soundInit();
//void musicDone();

void setupSoundtrack();
void startSoundtrack();
void stopSoundtrack();

void changeSoundtrack(char* changeTo);

#endif
