#pragma once
#include "PhysicsBody.h"
#include "Sprite.h"

class Health
{
private:
	unsigned int hID = 0;

	enum class HealthEnums
	{
		FULL,
		TWO,
		ONE,
		EMPTY
	};

public:
	Health();
	Health(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int);
	void InitHealth(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID);
	void update(unsigned int pID);
	void animUpdate(int hp);
};
