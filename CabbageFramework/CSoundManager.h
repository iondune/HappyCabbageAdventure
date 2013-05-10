#ifndef _CSOUNDMANAGER_INCLUDED_
#define _CSOUNDMANAGER_INCLUDED_

struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

#define MUSIC_DIR "../Media/Music/"
#include <map>
#include <string>

class CSoundManager {
private:
   std::map<std::string, Mix_Chunk*> *sounds;
   std::map<std::string, Mix_Music*> *tracks;
   bool audioInitialized;
   std::string currentTrack;

public:
   CSoundManager();
   void initAudio();
   void swapTrack(std::string);
   void playSound(std::string, int = 0);
   void stopSound(std::string);
   void closeAudio();

   void registerAndPlaySound(std::string, int = 0);
   void registerAndSwapTrack(std::string);
   
   void registerSound(std::string);
   void registerTrack(std::string);
   //void unregisterSound(std::string);

   bool isRegisteredTrack(std::string);
   bool isRegisteredSound(std::string);

   static std::string toLower(std::string);
};
#endif