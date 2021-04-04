#pragma once

#include "Trigger.h"
#include "BackEnd.h"

class Hook : public Trigger
{
public:
	Hook();
	
	void PassEntity(int ent);

	void OnTrigger() override;

	void Update() override;
	void OnEnter() override;
	void OnExit() override;


	bool isHookAttached() override;

protected:
	int speedScaling = 5;
	int myEntity = NULL;
	bool attached = false;
	bool triggered = false;
};

