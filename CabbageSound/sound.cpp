#include "sound.h"
#include <SDL/SDL.h>

//Used to run current soundtrack
Mix_Music *music;

Mix_Chunk *die;
Mix_Chunk *victory;
Mix_Chunk *jump;
Mix_Chunk *takeDmg;
Mix_Chunk *killEnemy;

bool playDead;
bool playVictory;
bool playTakeDmg;
bool playJump;
bool musicOn;

//Directory variables
std::string MusicDirectory = "../Media/Music/";


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
   playVictory = true;
   playTakeDmg = true;
   playJump = true;
   musicOn = true;
}

void setupSoundtrack(char *startMusic) {
   int audio_rate = 22050;
   Uint16 audio_format = AUDIO_S16;
   int audio_channels = 2;
   int audio_buffers = 4096;
   std::string temp;

   if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
      printf("Could not open audio!\n");

   //Pre-load pointers
   temp = MusicDirectory + startMusic;
   music = Mix_LoadMUS(temp.c_str());

   temp = MusicDirectory + "death.wav";
   die = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "victory.wav";
   victory = Mix_LoadWAV ("../Media/Music/victory.wav");

   temp = MusicDirectory + "jump.wav";
   jump = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "takeDmg.wav";
   takeDmg = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "hitEnemy.wav";
   killEnemy = Mix_LoadWAV(temp.c_str());

   //Decrease volume of jump
   Mix_VolumeChunk(jump, 64);
}

void startSoundtrack() {
   musicOn = true;
   Mix_PlayMusic(music, -1);
}

void stopSoundtrack() {
   if(musicOn) {
      musicOn = false;
      Mix_HaltMusic();
      Mix_FreeMusic(music);
   }
}


void changeSoundtrack(char* changeTo) {
   std::string temp;

   //Stop music from playing (if currently is)
   if (musicOn) {
      Mix_FadeOutMusic(100);
      Mix_FreeMusic(music);
   }

   temp = MusicDirectory + changeTo;

   music = Mix_LoadMUS(temp.c_str());

   Mix_FadeInMusic(music, -1, 2000);

   musicOn = true;
}
