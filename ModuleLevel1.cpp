#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include "ModuleLevel1.h"

ModuleLevel1::ModuleLevel1()
{}

ModuleLevel1::~ModuleLevel1()
{}

// Load assets
bool ModuleLevel1::Start()
{
	LOG("Loading space scene");
	
	background = App->textures->Load("Assets/Sprites/Maps/world_1-1.png");

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();
	App->enemies->Enable();

	//App->audio->PlayMusic("rtype/stage1.ogg", 1.0f);
	
	// Colliders ---
	{
		//floors
		App->collision->AddCollider({ 0, 200, 1104, 24 }, COLLIDER_WALL);
		//mystery boxes
		App->collision->AddCollider({ 256, 136, 16, 16 }, COLLIDER_MYSTERY_BOX);
		App->collision->AddCollider({ 352, 72, 16, 16 }, COLLIDER_MYSTERY_BOX);
		//blocks
		App->collision->AddCollider({ 321, 136, 79, 16 }, COLLIDER_WALL);
		//tubes
		//1
		App->collision->AddCollider({ 448, 168, 32, 14 }, COLLIDER_WALL); //up
		App->collision->AddCollider({ 450, 182, 28, 18 }, COLLIDER_WALL); //down
		//2
		App->collision->AddCollider({ 608, 152, 32, 14 }, COLLIDER_WALL); //up
		App->collision->AddCollider({ 610, 167, 28, 33 }, COLLIDER_WALL); //down
	}
	// Enemies ---
	{
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 600, 80);
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 625, 80);
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 640, 80);
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 665, 80);

		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 735, 120);
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 750, 120);
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 775, 120);
		//App->enemies->AddEnemy(ENEMY_TYPES::REDBIRD, 790, 120);

		//App->enemies->AddEnemy(ENEMY_TYPES::BROWNSHIP, 830, 100);
		//App->enemies->AddEnemy(ENEMY_TYPES::BROWNSHIP, 850, 100);
		//App->enemies->AddEnemy(ENEMY_TYPES::BROWNSHIP, 870, 100);
		//App->enemies->AddEnemy(ENEMY_TYPES::BROWNSHIP, 890, 100);

		App->enemies->AddEnemy(ENEMY_TYPES::MECH, 300, 184);
	}
	return true;
}

// UnLoad assets
bool ModuleLevel1::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(background);

	App->enemies->Disable();
	App->collision->Disable();
	App->particles->Disable();
	App->player->Disable();

	return true;
}

// Update: draw background
update_status ModuleLevel1::Update()
{
	// Move camera forward -----------------------------
	//App->render->camera.x += 1 * SCREEN_SIZE;

	// Draw everything --------------------------------------
	App->render->Blit(background, 0, 0, NULL);
	
	return UPDATE_CONTINUE;
}