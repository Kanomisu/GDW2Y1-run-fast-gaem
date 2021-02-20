#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}
/*
void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\

	//Idle Left
	m_animController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["IdleRight"].get<Animation>());
#ifdef TOPDOWN
	//Idle Up
	m_animController->AddAnimation(animations["IdleUp"].get<Animation>());
	//Idle Down
	m_animController->AddAnimation(animations["IdleDown"].get<Animation>());
#endif

	//Walk Animations\\

	//WalkLeft
	m_animController->AddAnimation(animations["WalkLeft"].get<Animation>());
	//WalkRight
	m_animController->AddAnimation(animations["WalkRight"].get<Animation>());
#ifdef TOPDOWN
	//WalkUP
	m_animController->AddAnimation(animations["WalkUp"].get<Animation>());
	//WalkDown
	m_animController->AddAnimation(animations["WalkDown"].get<Animation>());
#endif

	//Attack Animations\\

	//AttackLeft
	m_animController->AddAnimation(animations["AttackLeft"].get<Animation>());
	//AttackRight
	m_animController->AddAnimation(animations["AttackRight"].get<Animation>());
#ifdef TOPDOWN
	//AttackUp
	m_animController->AddAnimation(animations["AttackUp"].get<Animation>());
	//AttackDown
	m_animController->AddAnimation(animations["AttackDown"].get<Animation>());
#endif

	//Set Default Animation
	m_animController->SetActiveAnim(IDLELEFT);


}
*/

void Player::InitPlayer(Transform* transform, bool hasPhys, PhysicsBody* body, CanJump* jump, b2World* physWorld)
{
	m_transform = transform;
	m_hasPhysics = hasPhys;
	
	if (hasPhys)
	{
		m_physBody = body;
	}
	m_canJump = jump;
	m_physicsWorld = physWorld;
}

void Player::Update()
{
	MovementUpdate();
	//AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;
	float speed = 10.f;
	vec3 vel = vec3(0.f, 0.f, 0.f);
	float slideImpulse = 3000000000.f;
	float airSpeedMultiplier = 0.4f;

	if (m_canJump->m_canJump) {
		airSpeedMultiplier = 1.f;
	}
	if (Input::GetKeyDown(Key::LeftControl) && m_canJump->m_canJump) {
		if (m_facing == LEFT) {
			m_physBody->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-slideImpulse, 0), true);
		}
		if (m_facing == RIGHT) {
			m_physBody->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(slideImpulse, 0), true);
		}
	}
	else {
		if (Input::GetKey(Key::A))
		{
			vel = vel + vec3(-1.f, 0.f, 0.f);
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			vel = vel + vec3(1.f, 0.f, 0.f);
			m_facing = RIGHT;
			m_moving = true;
		}
	}
	m_physBody->SetVelocity(vel * speed * airSpeedMultiplier + m_physBody->GetVelocity());
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}
	else if (m_attacking)
	{
		activeAnimation = ATTACK;

		//Check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			m_locked = false;
			m_attacking = false;
			//Resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}



