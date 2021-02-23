#pragma once
#include "PhysicsBody.h"
#include "Sprite.h"
#include "Utilities.h"
class Background
{
private:
	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;

public:
	Background();
	Background(std::string& fileName, int width, int height, Sprite* sprite, Transform* transform);
	void InitBackground(std::string& fileName, int width, int height, Sprite* sprite, Transform* transform);
	void update();

};

