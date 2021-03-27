#include "checkpointTrigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"
#include "EffectManager.h"


//change so it gets the currentplayer entity and setrespawn is a function in the player class
int effect;
void checkpointTrigger::OnEnter()
{
	//get location of the object its attached to
	//set that location as respawn point


	Trigger::OnEnter();

	if (!triggered)
	{

		//currentScene->setSpawn(respawn);
		
		triggered = true;
	}

	//Find out whats touching the portal, add it to target entities
	//I wish I had a method that returned what entity is touching the object
	//Change location of target entity

}
void checkpointTrigger::OnExit() // literally do nothing
{

	Trigger::OnExit();
	triggered = false;
	EffectManager::RemoveEffect(effect);
}

void checkpointTrigger::SetScene(PhysicsPlayground* scene)
{
	currentScene = scene;
}

void checkpointTrigger::setRespawn(b2Vec2 vec)
{
	respawn = vec;
}