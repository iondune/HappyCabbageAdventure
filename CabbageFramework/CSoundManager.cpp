#include "CSoundManager.h"
#include "../CabbageSound/SDL_mixer.h"
#include <algorithm>
#include <stdexcept>


CSoundManager::CSoundManager() : audioInitialized(false) {
   initAudio();
}

void CSoundManager::initAudio() {
   if(audioInitialized)
      return;
   audioInitialized = true;
   sounds = new std::map<std::string, Mix_Chunk*>();
   tracks = new std::map<std::string, Mix_Music*>();

   if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) < 0) {
      fprintf(stderr, "Could not open audio!\n");
      audioInitialized = false;
   }
}

void CSoundManager::closeAudio() {
   audioInitialized = false;
   if(audioInitialized)
      Mix_CloseAudio();

   for(std::map<std::string, Mix_Chunk*>::iterator it = sounds->begin(); it != sounds->end(); ++it) {
      delete it->second;
   }
   sounds->clear();
   delete sounds;

   for(std::map<std::string, Mix_Music*>::iterator it = tracks->begin(); it != tracks->end(); ++it) {
      delete it->second;
   }
   tracks->clear();
   delete tracks;
}

void CSoundManager::playSound(std::string name) {
   if(!isRegisteredSound(name)) {
      fprintf(stderr, "Sound %s was not registered. Not playing.\n", name);
      return;
   }
   
   Mix_Chunk *sound = sounds->at(name);
   Mix_PlayChannel(-1, sound, 0);
}

void CSoundManager::swapTrack(std::string name) {
   if(!isRegisteredTrack(name)) {
      fprintf(stderr, "Track %s was not registered. Not playing.\n", name);
      return;
   }
   
   Mix_FadeOutMusic(250);
   Mix_Music *track = tracks->at(name);
   Mix_FadeInMusic(track, -1, 250);
   //Mix_PlayMusic(track, -1);
}

void CSoundManager::registerAndPlaySound(std::string name) {
   registerSound(name);
   playSound(name);
}

void CSoundManager::registerAndSwapTrack(std::string name) {
   registerTrack(name);
   swapTrack(name);
}

bool CSoundManager::isRegisteredTrack(std::string name) {
   try {
      Mix_Music *track = tracks->at(name);
      if(track != NULL)
         return true;
      else
         return false;
   }
   catch(std::out_of_range err) {
      return false;
   }
}

bool CSoundManager::isRegisteredSound(std::string name) {
   try {
      Mix_Chunk *sound = sounds->at(name);
      if(sound != NULL)
         return true;
      else
         return false;
   }
   catch(std::out_of_range err) {
      return false;
   }
}

void CSoundManager::registerSound(std::string name) {
   if(!audioInitialized) {
      fprintf(stderr, "Audio was not initialized. Didn't register %s.\n", name);
      return;
   }

   if(isRegisteredSound(name)) {
      return;
   }

   if(CSoundManager::toLower(name).find(".wav") != std::string::npos) {
      Mix_Chunk *newSound = Mix_LoadWAV((MUSIC_DIR + name).c_str());
      sounds->emplace(name, newSound);
   }
   else {
      fprintf(stderr, "Given sound file was not type WAV. Didn't register %s.\n", name);
      return;
   }
}

void CSoundManager::registerTrack(std::string name) {
   if(!audioInitialized) {
      fprintf(stderr, "Audio was not initialized. Didn't register %s.\n", name);
      return;
   }

   if(isRegisteredTrack(name)) {
      return;
   }

   if(CSoundManager::toLower(name).find(".ogg") != std::string::npos ||
         CSoundManager::toLower(name).find(".mp3") != std::string::npos ||
         CSoundManager::toLower(name).find(".wav") != std::string::npos) {
      Mix_Music *newTrack = Mix_LoadMUS((MUSIC_DIR + name).c_str());
      tracks->emplace(name, newTrack);
   }
   else {
      fprintf(stderr, "Given track file was not a recognized type. Didn't register %s.\n", name);
      return;
   }
}

std::string CSoundManager::toLower(std::string str1) {
   std::string str(str1);
   std::transform(str.begin(), str.end(), str.begin(), ::tolower);
   return str;
}