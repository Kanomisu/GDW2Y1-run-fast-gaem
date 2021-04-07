#pragma once
#include "PhysicsBody.h"
#include "Sprite.h"

class AnimObject
{
private:
	unsigned int aID = 0;


public:
	AnimObject();
	AnimObject(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int);
	void InitObject(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID);
	
};

