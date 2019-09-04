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
	position.x = 150;
	position.y = 183;
	jumpTime = 0;
	col = App->collision->AddCollider({0, 0, 12, 17}, COLLIDER_PLAYER, this);
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
		App->render->camera.y = position.y;
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
		if(App->render->camera.x >0)App->render->camera.x -= speed;
		current_animation = &forward;
		flip = SDL_FLIP_HORIZONTAL;
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
	default:
		break;
	}

	if(App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, COLLIDER_PLAYER_SHOT);	
	}

	/*if(App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_IDLE
	   && App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE)
		current_animation = &idle;
	*/

	col->SetPos(position.x, position.y);

	// Draw everything --------------------------------------
	if(destroyed == false)
		App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),flip);
	if (position.y == 150)
	{
	//LOG("Debug");
	}
	Gravity(183);
	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	/*if(c1 == col && destroyed == false && App->fade->IsFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->level_1, (Module*)App->scene_intro);

		App->particles->AddParticle(App->particles->explosion, position.x, position.y, COLLIDER_NONE, 150);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, COLLIDER_NONE, 220);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, COLLIDER_NONE, 670);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, COLLIDER_NONE, 480);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, COLLIDER_NONE, 350);

		destroyed = true;
	}*/
	switch (c2->type)
	{
	case COLLIDER_WALL:

		if ((position.y > c2->rect.y + c2->rect.h)|| (position.y > c2->rect.y + c2->rect.h-5))
		{
			vy = 0;
		}
		if (position.y < c2->rect.y)
		{
				position.y = c2->rect.y - current_animation->GetCurrentFrame().h-1;
				vy = 0;
				isFalling = false;
				state = IDLE;
				
		}
		/*
		if (position.y > 183){
			vy = 0;
			position.y = 183;
			state = IDLE;
			isFalling = false;
		}
		*/
	break;
	default:
		break;
	}
}

void ModulePlayer::Gravity(int floor) {
	jumpTime = (deltaTime - jumpMoment)/1000;
	if (isFalling == true) {
		position.y = position.y - vy * jumpTime + (0.5f * gravity * pow(jumpTime, 2));
	}
}