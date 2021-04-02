#pragma once
#include "Trigger.h"
#include "ECS.h"
#include "PhysicsPlayground.h"

class PhysicsPlayground;
class DeathTrigger :
    public Trigger
{
public:
    void OnEnter() override;
    void OnExit() override;
    void SetScene(PhysicsPlayground* scene);
    PhysicsPlayground* currentScene;
protected:
    bool triggered = false;
};

