#ifndef __ENEMY_GOOMBA_H__
#define __ENEMY_GOOMBA_H__

#include "Enemy.h"
#include "Path.h"

class Enemy_Goomba : public Enemy
{
private:
	Path path;
	iPoint original_pos;
	Animation move;
	Animation die;

public:

	Enemy_Goomba(int x, int y);
	void Move();
	void Die();
	void OnCollision(Collider* c);
};

#endif // __ENEMY_GOOMBA_H__