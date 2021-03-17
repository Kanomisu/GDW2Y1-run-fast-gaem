#pragma once
#include "Trigger.h"
#include "BackEnd.h"

class Attack : public Trigger
{
public:
	Attack();
	void Update();
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
private:
	bool triggered = false;
};