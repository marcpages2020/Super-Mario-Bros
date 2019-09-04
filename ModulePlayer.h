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
JUMP,
CROUCH
};

struct PlayerInput {

	bool pressing_A;
	bool pressing_D;
	bool pressing_W;
	bool pressing_S;
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
	State state;
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation jump;
	Animation crouch;
	iPoint position;
	Collider* col;
	PlayerInput player_input;
	bool destroyed = false;
	bool isFalling = false;
	float jumpSpeed = 7.50f;
	float gravity = 19.8f;
	float jumpTime = 0;
	float jumpMoment = 0;
	float deltaTime = 0;
};

#endif