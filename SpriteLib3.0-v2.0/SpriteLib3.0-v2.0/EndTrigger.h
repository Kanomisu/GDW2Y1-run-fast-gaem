#pragma once

#include "Trigger.h"

class EndTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
	void OnUpdate();

	int returnInt();

protected:

	int endGame = -1;
	float timer = 0;
	bool triggered = false;
};

