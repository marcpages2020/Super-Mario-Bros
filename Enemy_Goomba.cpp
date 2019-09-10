#include "Application.h"
#include "Enemy_Goomba.h"
#include "ModuleCollision.h"
#include "Globals.h"


Enemy_Goomba::Enemy_Goomba(int x, int y) : Enemy(x, y)
{
	move.PushBack({20, 2, 16, 16});
	move.PushBack({39, 2, 16, 16});
	move.speed = 0.1f;
	move.pingpong = true;

	die.PushBack({ 1,2,16,16 });
	die.pingpong = true;

	if (enemy_state == ENEMY_MOVE)
	{
		path.PushBack({ -0.3f, 0.0f }, 150, &move);
		path.PushBack({ 0.3f, 0.0f }, 150, &move);
	}

	if (enemy_state == ENEMY_DIE)
	{
		path.PushBack({ 0,0 }, 0, &die);
	}

	collider = App->collision->AddCollider({0, 0, 16, 16}, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
	
	original_pos.x = x;
	original_pos.y = y;
	
}

void Enemy_Goomba::Move()
{
	position = original_pos + path.GetCurrentPosition(&animation);
}

void Enemy_Goomba::Die() {

}

void Enemy_Goomba::OnCollision(Collider* c) {

}
