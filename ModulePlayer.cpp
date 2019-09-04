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

	/*
	// move upwards
	up.PushBack({100, 1, 32, 14});
	up.PushBack({132, 0, 32, 14});
	up.loop = false;
	up.speed = 0.1f;
	*/
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
	col = App->collision->AddCollider({position.x, position.y, 32, 16}, COLLIDER_PLAYER, this);
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

	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN)
	{
		App->render->camera.y = position.y;
	}

	player_input.pressing_W = App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN;
	player_input.pressing_A = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
	player_input.pressing_S = App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT;
	player_input.pressing_D = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;

	switch (state)
	{
	case IDLE:
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
			position.y--;
			state = JUMP;
			jumpMoment = SDL_GetTicks();
			isFalling = true;
		}
		else if (player_input.pressing_S)
		{
			state = CROUCH;
		}
		break;
	case FORWARD:
		if (!player_input.pressing_D)
		{
			state = IDLE;
		}
		break;
	case BACKWARD:
		if (!player_input.pressing_A)
		{
			state = IDLE;
		}
		break;
	case CROUCH:
		if (!player_input.pressing_S)
		{
			state = IDLE;
		}
		break;
	default:
		break;
	}
	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModulePlayer::Update()
{
	//position.x += 1; // Automatic movement

	int speed = 1;
	
	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		break;
	case FORWARD:
		position.x += speed;
		App->render->camera.x += speed;
		break;
	case BACKWARD:
		position.x -= speed;
		if(App->render->camera.x >0)App->render->camera.x -= speed;
		break;
	case JUMP:
			current_animation = &jump;
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
		App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));
	if (position.y == 150)
	{
	LOG("Debug");
	}
	Gravity(183);
	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if(c1 == col && destroyed == false && App->fade->IsFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->level_1, (Module*)App->scene_intro);

		App->particles->AddParticle(App->particles->explosion, position.x, position.y, COLLIDER_NONE, 150);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, COLLIDER_NONE, 220);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, COLLIDER_NONE, 670);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, COLLIDER_NONE, 480);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, COLLIDER_NONE, 350);

		destroyed = true;
	}
}

void ModulePlayer::Gravity(int floor) {
	jumpTime = (deltaTime - jumpMoment)/1000;
	if (position.y < floor)
	{
		position.y = position.y - jumpSpeed * jumpTime + (0.5f * gravity * pow(jumpTime,2));
	}
	else
	{
		state = IDLE;
		isFalling = false;
		position.y = floor;
		jump.Reset();
	}
}