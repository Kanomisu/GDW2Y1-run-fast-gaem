#pragma once

#include "Trigger.h"

class Hook : public Trigger
{
public:
	void Update();

	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
protected:
	bool attached = false;
	bool triggered = false;
};

