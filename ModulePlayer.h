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
	JUMP_FORWARD,
	JUMP_BACKWARD,
	CROUCH,
	DIE
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
	void Gravity();

public:
	State state;
	SDL_Texture* graphics = nullptr;
	Mix_Chunk* jump_fx = nullptr;
	Mix_Chunk* die_fx = nullptr;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation jump;
	Animation crouch;
	Animation die;
	iPoint position;
	iPoint last_position;
	Collider* col;
	Collider* floor_col;
	PlayerInput player_input;
	int floor_left;
	int floor_right;
	bool destroyed = false;
	bool isFalling = false;
	bool wasFalling = false;
	bool colliding = false;
	float vy = 0;
	double gravity = 21.8f;
	float jumpTime = 0;
	float jumpMoment = 0;
	float deltaTime = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

#endif