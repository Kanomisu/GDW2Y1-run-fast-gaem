#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();

	//Mouse control thing
	void MouseLocation(vec2 mousePos);

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void MouseMotion(SDL_MouseMotionEvent event);
	void MousePress(SDL_MouseButtonEvent event);

protected:
	//Mouse pos
	vec2 m_mousePos = vec2(0.f, 0.f);

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int puzzleWall1;
	int puzzleWall2;
};
