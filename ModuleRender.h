#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Point;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PostUpdate();
	update_status Update();
	update_status PreUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed = 1.0f);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

	void StartCameraShake(int duration, float magnitude);
	void UpdateCameraShake();

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;

private:
	//TODO 0: A new set of variables to handle the camera shake
	bool shaking = false;

	int shake_duration = 0.0f;
	int shake_timer = 0.0f;
	float shake_magnitude = 0.0f;

	SDL_Point camera_offset;
	//----------------------------------
};

#endif //__ModuleRenderer_H__