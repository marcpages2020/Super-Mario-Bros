#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleSlowdown.h"
#include "SDL/include/SDL.h"
#include "ModuleCollision.h"

ModulePlayer::ModulePlayer()
{
	// idle 
	idle.PushBack({19, 43, 13, 17});

	//forward 
	forward.PushBack({35,43,12,17});
	forward.PushBack({50,43,11,17 });
	forward.PushBack({64,43,15,17 });
	forward.speed = 0.15f;

	//jump 
	jump.PushBack({ 98,44,16,16 });

	//die 
	die.PushBack({229,43,14,17});

	// Move down
	/*
	down.PushBack({33, 1, 32, 14});
	down.PushBack({0, 1, 32, 14});
	down.loop = false;
	down.speed = 0.1f;
	*/
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Assets/sprites/Characters/mario.png");

	destroyed = false;
	position.x = 80;
	position.y = 183;
	jumpTime = 0;
	col = App->collision->AddCollider({0, 0, 12, 17}, COLLIDER_PLAYER, this);
	floor_col = App->collision->AddCollider({ 0,0,12,17 },COLLIDER_PLAYER_ATTACK, this);
	current_animation = &idle;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	App->collision->EraseCollider(col);

	return true;
}

update_status ModulePlayer::PreUpdate() {

	deltaTime = SDL_GetTicks();
	last_position = position;

	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN)
	{
		LOG("Debug");
		//App->render->camera.y = position.y;
	}

	player_input.pressing_W = App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN;
	player_input.pressing_A = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
	player_input.pressing_S = App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT;
	player_input.pressing_D = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;

	if (state == IDLE) {
		if (player_input.pressing_A)
		{
			state = BACKWARD;
		}
		else if (player_input.pressing_D)
		{
			state = FORWARD;
		}
		else if ((player_input.pressing_W) && (isFalling == false))
		{
			state = JUMP;
			vy = 8.5f;
			position.y--;
			jumpMoment = SDL_GetTicks();
			isFalling = true;
		}
		else if (player_input.pressing_S)
		{
			state = CROUCH;
		}
	}
	if (state == FORWARD) {
			if (!player_input.pressing_D)
			{
				state = IDLE;
			}
			if (player_input.pressing_W)
			{
				state = JUMP_FORWARD;
				vy = 8.5f;
				position.y--;
				jumpMoment = SDL_GetTicks();
				isFalling = true;
			}
		}
	if (state == BACKWARD){
			if (!player_input.pressing_A)
			{
				state = IDLE;
			}
		if (player_input.pressing_W)
		{
			state = JUMP_BACKWARD;
			vy = 8.5f;
			position.y--;
			jumpMoment = SDL_GetTicks();
			isFalling = true;
		}
	}
	if (state == CROUCH) {
		if (!player_input.pressing_S)
		{
			state = IDLE;
		}
	}
	if (state == JUMP)
	{
		if (player_input.pressing_D)
		{
			position.x++;
		}
		if (player_input.pressing_A)
		{
			position.x--;
		}
	}
	if (state == JUMP_FORWARD) {
		if (player_input.pressing_D)
		{
			position.x++;
		}
		if (player_input.pressing_A)
		{
			position.x--;
		}

	}
	if (state == JUMP_BACKWARD) {
		if (player_input.pressing_A)
		{
			position.x--;
		}
		if (player_input.pressing_D)
		{
			position.x++;
		}
	}
	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 1;
	last_position = position;

	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		//flip = SDL_FLIP_NONE;
		break;
	case FORWARD:
		position.x += speed;
		App->render->camera.x += speed;
		current_animation = &forward;
		flip = SDL_FLIP_NONE;
		break;
	case BACKWARD:
		position.x -= speed;
		current_animation = &forward;
		flip = SDL_FLIP_HORIZONTAL;
		//to be deleted
		App->render->camera.x -= speed;
		break;
	case JUMP:
		current_animation = &jump;
		break;
	case JUMP_FORWARD:
		current_animation = &jump;
		flip = SDL_FLIP_NONE;
		break;
	case JUMP_BACKWARD:
		current_animation = &jump;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case DIE:
		current_animation = &die;
		break;
	default:
		break;
	}
   
	/*
	if(App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, COLLIDER_PLAYER_ATTACK);	
	}
	*/

	/*if(App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_IDLE
	   && App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE)
		current_animation = &idle;
	*/

	col->SetPos(position.x, position.y);
	floor_col->SetPos(position.x, position.y + current_animation->GetCurrentFrame().h);
	floor_col->SetSize(current_animation->GetCurrentFrame().w, 4);
	// Draw everything --------------------------------------
	if(destroyed == false)
		App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),flip);

	Gravity();
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{

	if ((c1 == col)&&(state != DIE))
	{
		switch (c2->type)
		{
		case COLLIDER_WALL:

			position = last_position;

			//floor
			if (position.y + current_animation->GetCurrentFrame().h / 2 < c2->rect.y)
			{
				state = IDLE;
				position.y = c2->rect.y - current_animation->GetCurrentFrame().h;
				vy = 0;
				isFalling = false;
			}

			//ceiling
			if ((position.y > c2->rect.y + c2->rect.h) || (position.y > c2->rect.y + c2->rect.h - 5))
			{
				vy = 0;
				position.y = c2->rect.y + c2->rect.h;
			}

			break;
		case COLLIDER_MYSTERY_BOX:

			position = last_position;

			//mario on
			if (position.y + current_animation->GetCurrentFrame().h / 2 < c2->rect.y)
			{
				state = IDLE;
				position.y = c2->rect.y - current_animation->GetCurrentFrame().h;
				vy = 0;
				isFalling = false;
			}

			//mario under
			if ((position.y > c2->rect.y + c2->rect.h) || (position.y > c2->rect.y + c2->rect.h - 5))
			{
				vy = 0;
				position.y = c2->rect.y + c2->rect.h;
			}
			break;
		case COLLIDER_ENEMY:
			state = DIE;
			vy = 6;
			isFalling = true;
			jumpMoment = SDL_GetTicks();
			//position.y -= 30;
			break;
		default:
			break;
		}
	}
	else if (c1 == floor_col)
	{
		if (c2->type == COLLIDER_ENEMY)
		{
			c2 == nullptr;
		}
	}
}

void ModulePlayer::Gravity() {
	jumpTime = (deltaTime - jumpMoment)/1000;

	if (isFalling == true) {
		position.y = position.y - vy * jumpTime + (0.5f * gravity * pow(jumpTime, 2));
	}
}



