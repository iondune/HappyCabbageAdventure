#include "SDL_mixer.h"

/*SDL_Mixer Code*/
Mix_Music *music = NULL;

void musicDone();

void setupSoundtrack() {
   int audio_rate = 22050;
   Uint16 audio_format = AUDIO_S16;
   int audio_channels = 2;
   int audio_buffers = 4096;

   if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
      printf("Could not open audio!\n");

   music = Mix_LoadMUS("SMW.wav");

   Mix_PlayMusic(music, -1);
}
