#include "DeathTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"
void DeathTrigger::OnEnter()
{
	//get location of the object its attached to
	//set that location as respawn point


	Trigger::OnEnter();

	if (!triggered)
	{

		currentScene->respawnPlayer();

		triggered = true;
	}
}

void DeathTrigger::OnExit() // literally do nothing
{

	Trigger::OnExit();
	triggered = false;

}

void DeathTrigger::SetScene(PhysicsPlayground * scene)
{
	currentScene = scene;
}
