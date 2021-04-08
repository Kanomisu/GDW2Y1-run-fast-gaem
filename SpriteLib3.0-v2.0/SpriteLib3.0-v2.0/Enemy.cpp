#include "Enemy.h"  
#include "Utilities.h"

void Enemy::Init(std::string& fileName, std::string& animJSON, int width, int height, Sprite* sprite, AnimationController* animCon, Transform* transform, PhysicsBody* physBody, float leftXStation, float rightXStation, unsigned int ent) {
	m_sprite = sprite;
	m_animationController = animCon;
	m_transform = transform;
	m_physBody = physBody;

	posLeft = leftXStation;
	posRight = rightXStation;

	m_entityID = ent;
	

	//Initialize UVs
	m_animationController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animationController);
	m_animationController->SetVAO(m_sprite->GetVAO());
	m_animationController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animJSON);

	//IDLE ANIMATIONS\\

	//Idle Left
	m_animationController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//Idle Right
	m_animationController->AddAnimation(animations["IdleRight"].get<Animation>());

	//Moving Animations\\

	//MoveLeft
	m_animationController->AddAnimation(animations["MoveLeft"].get<Animation>());
	//MoveRight
	m_animationController->AddAnimation(animations["MoveRight"].get<Animation>());

	//Death Animations\\

	//DeathLeft
	m_animationController->AddAnimation(animations["DeathLeft"].get<Animation>());
	//DeathRight
	m_animationController->AddAnimation(animations["DeathRight"].get<Animation>());

	//m_physBody->GetBody()->SetAwake(false); //In this line, make it so that the Physics are asleep.
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
	//std::cout << "AI Update" << std::endl;
	//Fuck this framework
	m_sprite = &ECS::GetComponent<Sprite>(m_entityID);
	//m_animationController = &ECS::GetComponent<AnimationController>(m_entityID);
	m_transform = &ECS::GetComponent<Transform>(m_entityID);
	m_physBody = &ECS::GetComponent<PhysicsBody>(m_entityID);

	if (m_dead) {
		m_awake = false;
		m_physBody->GetBody()->SetAwake(false);
	}
	else {
		//Awake Check
		vec2 eToP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - m_physBody->GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - m_physBody->GetPosition().y);
		if (eToP.GetMagnitude() < m_awakenDistance) {
			//std::cout << "Amogus" << std::endl;
			m_awake = true;
			m_physBody->GetBody()->SetAwake(true); //Awaken the Physics Body
		}
		else {
			//std::cout << "SUS" << std::endl;
			m_awake = false;
			m_physBody->GetBody()->SetAwake(false); //Schleep the MF Phys Body
		}

		if (m_awake) {
			//Check which behaviour needs to be updated.  
			switch (m_state) {
			case DAMAGED:
				//std::cout << "Damaged" << std::endl;
				killEnemy();
				break;
			case THINKING:
				//std::cout << "Big Thonk" << std::endl;
				thinking();
				break;
			case MOVELEFT:
				//std::cout << "Patrol Left" << std::endl;
				moveLeft();
				break;
			case MOVERIGHT:
				//std::cout << "Patrol Right" << std::endl;
				moveRight();
				break;
			case ATTACKING:
				//std::cout << "Attacking" << std::endl;
				attack();
				break;
			}
		}
	}
}

void Enemy::moveLeft()
{
	if (m_physBody->GetPosition().x > posLeft) {
		m_physBody->SetVelocity(vec3(-m_speed, 0, 0));
	}
	else {
		m_physBody->SetVelocity(vec3(0, 0, 0));
		m_state = THINKING;
		m_facing = left;
		m_timer = m_moveCooldown;
	}
	m_facing = left;
	m_AnimState = MOVING;
}

void Enemy::moveRight()
{
	if (m_physBody->GetPosition().x < posRight) {
		m_physBody->SetVelocity(vec3(m_speed, 0, 0));
	}
	else {
		m_state = THINKING;
		m_facing = right;
		m_timer = m_moveCooldown;
	}
	m_facing = right;
	m_AnimState = MOVING;
}

void Enemy::attack()
{
	m_physBody = &ECS::GetComponent<PhysicsBody>(m_entityID);
	if (m_timer > 0) {
		vec2 EtoP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - m_physBody->GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - m_physBody->GetPosition().y);
		double EtoPAngle = atan2(EtoP.x, EtoP.y) * 57.29577951308 * -1;
		vec3 movement = vec3(m_attackSpeed * sin(-EtoPAngle * 0.01745329f), m_attackSpeed * cos(-EtoPAngle * 0.01745329f), 0);
		m_physBody->SetVelocity(movement);
		m_timer -= Timer::deltaTime;

		//This sets the enemies direction correctly when they're attacking (animation-wise anyways)
		if (m_physBody->GetVelocity().x > 0)
		{
			m_facing = right;
		}
		else if (m_physBody->GetVelocity().x < 0)
		{
			m_facing = left;
		}

	}
	else {
		m_state = THINKING;
		m_timer = m_attackCooldown;
	}
	m_AnimState = MOVING;
}

void Enemy::thinking()
{
	m_physBody->SetVelocity(vec3(0, 0, 0));
	if (m_timer > 0) {
		m_timer -= Timer::deltaTime;
	}
	else {
		vec2 dToP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - m_physBody->GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - m_physBody->GetPosition().y);
		//std::cout << "Distance to Player: " << dToP.GetMagnitude() << std::endl;
		if (dToP.GetMagnitude() < m_engagementRange) {
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
	m_AnimState = Idle;

}

void Enemy::killEnemy()
{
	if (m_timer > 0) {
		//Animate the enemy dying 
		m_timer -= Timer::deltaTime;
		m_AnimState = DYING;
	}
	else {
		m_dead = true;
		m_sprite->SetTransparency(0.f);
		m_physBody->SetPosition(b2Vec2(-4000, 100));
		m_physBody->GetBody()->SetAwake(false); //Schleep the body
	}
}

void Enemy::setDeath()
{
	m_state = DAMAGED;
	m_timer = m_deathTimer;
}


void Enemy::AnimationUpdate()
{
	m_animationController = &ECS::GetComponent<AnimationController>(m_entityID);
	int activeAnimation = 0;
	if (m_AnimState == DYING)
	{
		activeAnimation = DYING;
		if (m_animationController->GetAnimation(m_animationController->GetActiveAnim()).GetAnimationDone())
		{
			activeAnimation = Idle;
		}
	}
	activeAnimation = m_AnimState;
	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Enemy::SetActiveAnimation(int anim)
{
	m_animationController->SetActiveAnim(anim);
}
