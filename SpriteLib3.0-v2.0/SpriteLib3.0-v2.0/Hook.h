#pragma once

#include "Trigger.h"
#include "BackEnd.h"

class Hook : public Trigger
{
public:
	Hook();
	void Update();
	void PassEntity(int ent);

	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
protected:
	int myEntity = NULL;
	bool attached = false;
	bool triggered = false;
};

