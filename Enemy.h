#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

enum Enemy_State {
	ENEMY_MOVE,
	ENEMY_ATTACK,
	ENEMY_DIE
};

class Enemy
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;

public:
	iPoint position;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Die() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);

	Enemy_State enemy_state;

	bool dying = false;
	int die_counter = 20;
};

#endif // __ENEMY_H__