#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

enum State {
IDLE,
FORWARD,
BACKWARD,
JUMP
};

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	update_status PreUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void Gravity(int floor = 183);

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation jump;
	Animation crouch;
	iPoint position;
	Collider* col;
	bool destroyed = false;
	bool isFalling = false;
	float jumpSpeed = 2;
	float gravity;
	float jumpTime = 0;
};

#endif