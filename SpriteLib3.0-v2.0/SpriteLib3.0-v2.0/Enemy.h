#pragma once  
#include"PhysicsBody.h"
#include "Sprite.h"
#include "Timer.h" 
enum States {
	MOVELEFT,
	MOVERIGHT,
	ATTACKING,
	THINKING,
	DAMAGED
};
//ngl i kinda hate you for doing this but whatever
//All the enums for this are specifically for animations
enum animStates {
	Idle = 0,
	MOVING = 2,
	DYING = 4
};
enum AnimDirE
{
	left,
	right
};
/*
enum AnimEnums
{
	IDLELEFT,
	IDLERIGHT,

	MOVINGLEFT,
	MOVINGRIGHT,

	DEATHLEFT,
	DEATHRIGHT
};
*/

class Enemy
{
private:
	Transform* m_transform = nullptr;
	Sprite* m_sprite = nullptr;
	AnimationController* m_animationController = nullptr;
	PhysicsBody* m_physBody = nullptr;

	unsigned int m_entityID;

	bool m_dead = false;

	bool m_awake = false;

	//Variables  
	float m_speed = 10.f;
	float m_attackSpeed = 100.f;
	float m_engagementRange = 200.f;

	//States  
	int m_state = THINKING;
	int m_AnimState = Idle;
	float m_timer = 0; //Used to timed behaviours  
	float m_attackCooldown = 0.8f;
	float m_attackLength = 0.7f;
	float m_deathTimer = 1.24f; //Set this to the length of the death animation.
	float m_moveCooldown = 1.0f;
	float m_awakenDistance = 400.f;

	//Positions  
	float posLeft = 0.f;
	float posRight = 0.f;

	void SetActiveAnimation(int anim);

	//Default animation direction
	AnimDirE m_facing = right;
public:
	void Init(std::string& fileName, std::string& animations, int width, int height, Sprite* sprite, AnimationController* animCon, Transform* transform, PhysicsBody* physBody, float leftXStation, float rightXStation, unsigned int ent);
	void Init(Sprite* sprite, Transform* transform, PhysicsBody* physBody, float leftXStation, float rightXStation, unsigned int ent); //Testing ONLY, do not use
	void Update();

	//Behaviours  
	//Movement  
	void moveLeft();
	void moveRight();
	//Attack  
	void attack();
	//Big Thonk  
	void thinking();
	//Dying  
	void killEnemy();
	void setDeath();
	void AnimationUpdate();
};