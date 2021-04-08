#pragma once
#include "Trigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"

class PhysicsPlayground;
class checkpointTrigger :
    public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;

	void SetScene(PhysicsPlayground* scene);

	void setRespawn(b2Vec2 vec);
	PhysicsPlayground* currentScene;
	b2Vec2 respawn;
protected:

	bool triggered = false;
};

