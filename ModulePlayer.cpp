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

	// Move down
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
	
	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModulePlayer::Update()
{
	//position.x += 1; // Automatic movement

	int speed = 1;
	
	if(App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	{
		position.x -= speed;
		App->render->camera.x -= speed;

	}

	if(App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	{
		position.x += speed;
		App->render->camera.x += speed;
	}

	if(App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		/*position.y += speed;
		if(current_animation != &down)
		{
			down.Reset();
			current_animation = &down;
		}*/
	}
	
	if(App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
	{
		position.y -= speed;
		if((current_animation != &jump)&&(isFalling == false))
		{
			jump.Reset();
			current_animation = &jump;
			jumpTime = SDL_GetTicks();
		}
	}

	if(App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, COLLIDER_PLAYER_SHOT);
		//TODO 3: Call the camera shake whenever the players shoots a laser	
	}

	if(App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_IDLE
	   && App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE)
		current_animation = &idle;
	

	col->SetPos(position.x, position.y);

	// Draw everything --------------------------------------
	if(destroyed == false)
		App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));
	
	Gravity();
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
	jumpTime = SDL_GetTicks() - jumpTime;
	if (position.y < floor)
	{
		position.y = position.y + jumpSpeed*jumpTime + 1/2 * gravity * pow(jumpTime,2);
	}
}