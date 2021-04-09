#include "Hook.h"

Hook::Hook()
{

}
void Hook::PassEntity(int ent)
{
	myEntity = ent;
}
void Hook::Update()
{
	Trigger::Update();
	if (attached)
	{
		ECS::GetComponent<PhysicsBody>(Trigger::GetTriggerEntity()).GetBody()->SetLinearVelocity(b2Vec2(0,0)); //freeze the hook to the wall

		b2Vec2 hookPos = ECS::GetComponent<PhysicsBody>(Trigger::GetTriggerEntity()).GetPosition();
		b2Vec2 playerPos = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition();
		vec3 pullDirection = vec3(hookPos.x - playerPos.x, hookPos.y - playerPos.y, 0); //create a vector that 'points' from the player to the hook, apply that as a force
		speedScaling *= speedScaling;
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).ApplyForce(pullDirection * (20000)); // vector naturally gets smaller as player approaches the wall, making the pull start strong and end weaker.
		//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetVelocity(pullDirection *20);
		//add a scaling variable that gets exponentially stronger with time
	}
}

bool Hook::isHookAttached()
{
	return attached;
}

void Hook::OnTrigger()
{
	
}

void Hook::OnEnter()
{
	Trigger::OnEnter();
	//std::cout << "Hook attached\n";
	attached = true;
}

void Hook::OnExit()
{
}

