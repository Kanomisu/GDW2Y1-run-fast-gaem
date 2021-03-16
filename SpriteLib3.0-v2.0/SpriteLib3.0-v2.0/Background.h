#pragma once
#include "PhysicsBody.h"
#include "Sprite.h"
#include "Utilities.h"
class Background
{
private:

	/*
	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;
	*/
	unsigned int bgID = 0;

public:
	Background();
	//Background(std::string& fileName, int width, int height, Sprite* sprite, Transform* transform);
	//void InitBackground(std::string& fileName, int width, int height, Sprite* sprite, Transform* transform, unsigned int bgID);
	Background(std::string& fileName, int width, int height, unsigned int );
	void InitBackground(std::string& fileName, int width, int height, unsigned int ID);
	void update();
	void attachTransform(Transform* transform);
	void attachSprite(Sprite* sprite);

};

