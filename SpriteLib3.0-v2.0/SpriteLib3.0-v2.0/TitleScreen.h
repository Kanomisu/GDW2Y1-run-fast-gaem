#pragma once

#include "Scene.h"
#include "Tone Fire/ToneFire.h"
class TitleScreen : public Scene
{

public:
	TitleScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Mouse Input
	void MouseClick(SDL_MouseButtonEvent evnt)override;
	void MouseMotion(SDL_MouseMotionEvent event);

protected:
	vec2 m_mousePos = vec2(0.f, 0.f);

	ToneFire::FMODCore fmod{};

	bool menu = true;
	bool play = false;
	bool displayCredits = false;
	bool inputCredits = false;
	bool setScene = false;

	float timer = 0;
	int loading = 1;
	int credits = 2;
};

