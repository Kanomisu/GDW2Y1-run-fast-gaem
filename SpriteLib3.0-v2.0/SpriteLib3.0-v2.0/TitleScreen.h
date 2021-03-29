#pragma once

#include "Scene.h"
class TitleScreen : public Scene
{

public:
	TitleScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Mouse Input
	//Because these are virtual you can override them in your inherited classes.
	//The same way you do for Update().
	void MouseClick(SDL_MouseButtonEvent evnt)override;

	void MouseMotion(SDL_MouseMotionEvent event);

	void KeyboardDown();

protected:
	vec2 m_mousePos = vec2(0.f, 0.f);

};

