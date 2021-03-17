#include "Enemy.h"  
#include "Utilities.h"

void Enemy::Init(Sprite* sprite, AnimationController* animCon, Transform* transform, PhysicsBody* physBody, float leftXStation, float rightXStation, unsigned int ent) {
	m_sprite = sprite;
	m_animationController = animCon;
	m_transform = transform;
	m_physBody = physBody;

	posLeft = leftXStation;
	posRight = rightXStation;

	m_entityID = ent;
	m_physBody->GetBody()->SetAwake(false); //In this line, make it so that the Physics are asleep.
}

void Enemy::Init(Sprite* sprite, Transform* transform, PhysicsBody* physBody, float leftXStation, float rightXStation, unsigned int ent) {
	m_sprite = sprite;
	m_transform = transform;
	m_physBody = physBody;

	posLeft = leftXStation;
	posRight = rightXStation;

	m_entityID = ent;
}

void Enemy::Update()
{
	//Fuck this framework
	m_sprite = &ECS::GetComponent<Sprite>(m_entityID);
	//m_animationController = &ECS::GetComponent<AnimationController>(m_entityID);
	m_transform = &ECS::GetComponent<Transform>(m_entityID);
	m_physBody = &ECS::GetComponent<PhysicsBody>(m_entityID);

	//Awake Check
	vec2 eToP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - m_physBody->GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - m_physBody->GetPosition().y);
	if (eToP.GetMagnitude() < m_awakenDistance) {
		m_awake = true;
		m_physBody->GetBody()->SetAwake(true); //Awaken the Physics Body
	}
	else {
		m_awake = false;
		m_physBody->GetBody()->SetAwake(false); //Schleep the MF Phys Body
	}

	if(m_awake) {
		//Check which behaviour needs to be updated.  
		switch (m_state) {
		case DAMAGED:
			killEnemy();
			break;
		case THINKING:
			thinking();
			break;
		case MOVELEFT:
			moveLeft();
			break;
		case MOVERIGHT:
			moveRight();
			break;
		case ATTACKING:
			attack();
			break;
		}
	}
}

void Enemy::moveLeft()
{
	if (m_physBody->GetPosition().x > posLeft) {
		m_physBody->SetVelocity(vec3(-m_speed, 0, 0));
	}
	else {
		m_state = THINKING;
		m_timer = m_moveCooldown;
	}
}

void Enemy::moveRight()
{
	if (m_physBody->GetPosition().x < posRight) {
		m_physBody->SetVelocity(vec3(m_speed, 0, 0));
	}
	else {
		m_state = THINKING;
		m_timer = m_moveCooldown;
	}
}

void Enemy::attack()
{
	if (m_timer > 0) {
		vec2 EtoP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - m_physBody->GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - m_physBody->GetPosition().y);
		double EtoPAngle = atan2(EtoP.x, EtoP.y) * 57.29577951308 * -1;
		vec3 movement = vec3(m_attackSpeed * sin(-EtoPAngle * 0.01745329f), m_attackSpeed * cos(-EtoPAngle * 0.01745329f), 0);
		m_physBody->SetVelocity(movement);
		m_timer -= Timer::deltaTime;
	}
	else {
		m_state = THINKING;
		m_timer = m_attackCooldown;
	}
}

void Enemy::thinking()
{
	if (m_timer > 0) {
		m_timer -= Timer::deltaTime;
	}
	else {
		int x = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - m_physBody->GetPosition().x;
		int y = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - m_physBody->GetPosition().y;
		float dToP = sqrt(x ^ 2 + y ^ 2);
		//Decide which option to do.  
		if (dToP < m_engagementRange) {
			m_state = ATTACKING;
			m_timer = m_attackLength;
		}
		else if (m_physBody->GetPosition().x < posRight) {
			m_state = MOVERIGHT;
		}
		else if (m_physBody->GetPosition().x > posLeft) {
			m_state = MOVELEFT;
		}
	}

}

void Enemy::killEnemy()
{
	if (m_timer > 0) {
		//Animate the enemy dying 
		m_timer -= Timer::deltaTime;
	}
	else {
		m_physBody->SetPosition(b2Vec2(0, -900));
		m_physBody->GetBody()->SetAwake(false); //Schleep the body
	}
}

void Enemy::setDeath()
{
	m_state = DAMAGED;
	m_timer = m_deathTimer;
}
