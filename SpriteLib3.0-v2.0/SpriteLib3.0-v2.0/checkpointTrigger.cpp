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

		currentScene->setRespawn(respawn);
		
		triggered = true;
	}


}
void checkpointTrigger::OnExit() // literally do nothing
{

	Trigger::OnExit();
	triggered = false;
	
}

void checkpointTrigger::SetScene(PhysicsPlayground* scene)
{
	currentScene = scene;
}

void checkpointTrigger::setRespawn(b2Vec2 vec)
{
	respawn = vec;
}