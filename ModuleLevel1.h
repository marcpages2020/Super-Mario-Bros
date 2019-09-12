#ifndef __MODULELEVEL1_H__
#define __MODULELEVEL1_H__

#include "Module.h"

struct SDL_Texture;

class ModuleLevel1 : public Module
{
public:
	ModuleLevel1();
	~ModuleLevel1();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* background = nullptr;
	SDL_Texture* stars = nullptr;
	Mix_Music* music;
};

#endif // __MODULELEVEL1_H__