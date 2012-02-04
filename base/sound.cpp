#include "sound.h"

void soundInit() {
   //Used to run current soundtrack
   music = NULL;

   //Sound Effects
   die = NULL;
   jump = NULL;
   takeDmg = NULL;
   killEnemy = NULL;

   //Misc Sound Variables
   playDead = true;
   playTakeDmg = true;
   playJump = true;
   musicOn = true;
}

void setupSoundtrack() {
   int audio_rate = 22050;
   Uint16 audio_format = AUDIO_S16;
   int audio_channels = 2;
   int audio_buffers = 4096;

   if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
      printf("Could not open audio!\n");

   //Pre-load pointers
   music = Mix_LoadMUS("sounds/SMW.wav");
   die = Mix_LoadWAV("sounds/death.wav");
   jump = Mix_LoadWAV("sounds/jump.wav");
   takeDmg = Mix_LoadWAV("sounds/takeDmg.wav");
   killEnemy = Mix_LoadWAV("sounds/hitEnemy.wav");

   //Decrease volume of jump
   printf("Prev Volume: %d\n", Mix_VolumeChunk(jump, 64));
}

void startSoundtrack() {
   Mix_PlayMusic(music, -1);
}

void stopSoundtrack() {
   musicOn = false;
   Mix_HaltMusic();
   Mix_FreeMusic(music);
}


void changeSoundtrack(char* changeTo) {
   //Stop music from playing (if currently is)
   if (musicOn)
      stopSoundtrack();

   music = Mix_LoadMUS(changeTo);

   Mix_PlayMusic(music, -1);
}
