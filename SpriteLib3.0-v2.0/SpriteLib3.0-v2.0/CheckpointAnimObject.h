#pragma once
#include "PhysicsBody.h"
#include "Sprite.h"

class CheckpointAnimObject
{
private:
	unsigned int aID = 0;


public:
	CheckpointAnimObject();
	CheckpointAnimObject(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID);
	void InitObject(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID);
	void SetActiveAnimation(int anim);
};

