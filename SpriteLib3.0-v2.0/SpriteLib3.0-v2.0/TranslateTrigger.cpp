#include "TranslateTrigger.h"
#include "ECS.h"

void TranslateTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void TranslateTrigger::OnEnter()
{
	Trigger::OnEnter();
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		//ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).GetPosition() + movement, true);
		//ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).ScaleBody(1.2f, 0, true);
		ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(movement, true);
	}
}

void TranslateTrigger::OnExit()
{
	Trigger::OnExit();
}
