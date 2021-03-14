#pragma once
#include "Scene.h"
class TestScene : public Scene
{
public:
	TestScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;




	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;


	//Mouse Input
	//Because these are virtual you can override them in your inherited classes.
	//The same way you do for Update().
	void MouseClick(SDL_MouseButtonEvent evnt)override;

	std::vector<b2Vec2> positions;


protected:

};

