#pragma once  
#include"PhysicsBody.h"
#include "Sprite.h"
#include "Timer.h" 
enum States {
	MOVELEFT,
	MOVERIGHT,
	ATTACKING,
	THINKING,
	DAMAGED,
};
class Enemy
{
private:
	Transform* m_transform = nullptr;
	Sprite* m_sprite = nullptr;
	AnimationController* m_animationController = nullptr;
	PhysicsBody* m_physBody = nullptr;

	unsigned int m_entityID;

	bool m_awake = false;

	//Variables  
	float m_speed = 10.f;
	float m_attackSpeed = 100.f;
	float m_engagementRange = 20.f;

	//States  
	int m_state = THINKING;
	float m_timer = 0; //Used to timed behaviours  
	float m_attackCooldown = 0.8f;
	float m_attackLength = 0.7f;
	float m_deathTimer = 1.24f; //Set this to the length of the death animation.
	float m_moveCooldown = 1.0f;
	float m_awakenDistance = 50.f;

	//Positions  
	float posLeft = 0.f;
	float posRight = 0.f;
public:
	void Init(Sprite* sprite, AnimationController* animCon, Transform* transform, PhysicsBody* physBody, float leftXStation, float rightXStation, unsigned int ent);
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
};