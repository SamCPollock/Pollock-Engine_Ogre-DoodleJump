#include "SoundManager.h"
#include <SDL.h>

/// This is the SoundManager constructor 
/// It sets the audio rate, format, channels, and buffers. 
SoundManager::SoundManager()
{
	SDL_Init(SDL_INIT_AUDIO);

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		// error. 
		exit(-1);
	}
}

SoundManager::~SoundManager()
{
	SDL_Quit();
}

/// This is the AddSoundEffect function.
/// It takes in the path of an audio file, then adds it to the sound effect bank. 
/// @note the index of chunks starts at 0. 

void SoundManager::addSoundEffect(const char* path)
{
	Mix_Chunk* tempChunk = Mix_LoadWAV(path);

	if (tempChunk != nullptr)
	{
		mSoundEffectBank.push_back(tempChunk);
	}
}

/// This is the play sound effect function. 
/// <param name="index">It takes in an integer for the index of the sound you want</param>
/// <param name="isLooping"> Next, it takes a boolean for whether the sound is supposed to loop or not.</param>
void SoundManager::playSoundEffect(const int index, bool isLooping)
{
	if (index > mSoundEffectBank.size() - 1)
	{
		// error
	}
	if (!isLooping)
		Mix_PlayChannel(-1, mSoundEffectBank[index], 0);
	else
		Mix_PlayChannel(-1, mSoundEffectBank[index], -1);


}
