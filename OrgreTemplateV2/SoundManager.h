#pragma once
#include <vector>
#include <SDL_mixer.h>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void addSoundEffect(const char* path);

	void playSoundEffect(const int index, bool isLooping);


private:
	std::vector<Mix_Chunk*> mSoundEffectBank;
};

