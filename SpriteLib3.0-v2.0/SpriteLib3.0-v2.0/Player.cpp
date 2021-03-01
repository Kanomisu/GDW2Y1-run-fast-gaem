#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body, CanJump* jump)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body, jump);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body, CanJump* jump)
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
	m_canJump = jump;
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



	//Dash Animations\\

	//DashLeft
	m_animController->AddAnimation(animations["DashLeft"].get<Animation>());
	//DashRight
	m_animController->AddAnimation(animations["DashRight"].get<Animation>());


	//Jump Animations\\

	//WalkLeft
	m_animController->AddAnimation(animations["JumpLeft"].get<Animation>());
	//WalkRight
	m_animController->AddAnimation(animations["JumpRight"].get<Animation>());
	//Attack Animations\\
	
	/*
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
	*/
	//Set Default Animation
	m_animController->SetActiveAnim(IDLERIGHT);


}


void Player::InitPlayer(Transform* transform, bool hasPhys, PhysicsBody* body, CanJump* jump)
{
	m_transform = transform;
	m_hasPhysics = hasPhys;
	
	if (hasPhys)
	{
		m_physBody = body;
	}
	m_canJump = jump;
	
}

void Player::Update()
{
	PlayerSpeedLevel();
	MovementUpdate();

	AnimationUpdate();
}

void Player::MovementUpdate()
{
	//Set up Vars
	m_moving = false;
	float vel = 0.f;
	float slideImpulse = 300.f;
	float airSpeedMultiplier = 1.f; //TODO Make it so you can adjust your air speed only slightly
	float friction = 0.8f;


	//Checks if grounded and resets some stuff
	if (m_canJump->m_canJump) {
		airSpeedMultiplier = 1.f;
	}
	if (!m_locked) //Checks if the player should be able to control their character
	{
		if (Input::GetKeyDown(Key::LeftControl) && m_canJump->m_canDash)
		{
			m_locked = true;
			m_canJump->m_canDash = false;
			m_dashing = true;
			m_dashTimeDelta = m_dashTime;
			std::cout << "Dashing" << std::endl;
		}
		else
		{
			if (Input::GetKey(Key::A))
			{
				vel = vel - 1.f;
				m_facing = LEFT;
				m_moving = true;
			}
			if (Input::GetKey(Key::D))
			{
				vel = vel + 1.f;
				m_facing = RIGHT;
				m_moving = true;
			}

			if (Input::GetKey(Key::Space) && m_canJump->m_canJump)
			{
				m_jumping = true;
				m_locked = true;
				m_physBody->SetVelocity(vec3(0.f, m_jumpHeight, 0.f));
				m_canJump->m_canJump = false;
			}
		}
		m_physBody->SetVelocity(vec3(vel * m_playerSpeed * airSpeedMultiplier, m_physBody->GetVelocity().y, m_physBody->GetVelocity().z));
	}
	if (m_dashing) {
		if (m_facing == RIGHT) {
			m_physBody->SetVelocity(vec3(m_dashSpeed, 0, 0));
		}
		else {
			m_physBody->SetVelocity(vec3(-m_dashSpeed, 0, 0));
		}
		m_dashTimeDelta -= Timer::deltaTime;
		if (m_dashTimeDelta < 0) {
			m_dashing = false;
			m_locked = false;
			//m_canJump->m_canJump = true; //TODO Add ability to jump out of air dash (extra jump)
		}
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		if (m_jumping) {
			activeAnimation = JUMP;
			if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
			{
				//will autoset to idle
				m_locked = false;
				m_jumping = false;
				//Resets jump animation
				m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();
				activeAnimation = WALK;
			}
		}
		if (m_dashing) {
			activeAnimation = DASH;
			if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
			{
				//will autoset to idle
				m_locked = false;
				//m_dashing = false;
				//Resets jump animation
				//m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();
				activeAnimation = WALK;
			}

			
		}
		else {
		
			activeAnimation = WALK;
		}
	}
	else if (m_jumping)
	{
		activeAnimation = JUMP;

		//check if anim is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//will autoset to idle
			m_locked = false;
			m_jumping = false;
			//Resets jumo animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();
			activeAnimation = IDLE;
		}
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
	else if (m_dashing)
	{
		activeAnimation = DASH;

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

void Player::PlayerSpeedLevel()
{
	//int level = 1;
	if (Input::GetKeyDown(Key::U)) {
		m_playerSpeed = 10.f;
		std::cout << "Key Pressed" << std::endl;
	}
	if (Input::GetKeyDown(Key::I)) {
		m_playerSpeed = 20.f;
		std::cout << "Key Pressed" << std::endl;
	}
	if (Input::GetKeyDown(Key::O)) {
		m_playerSpeed = 40.f;
		std::cout << "Key Pressed" << std::endl;
	}
	if (Input::GetKeyDown(Key::L)) {
		m_playerSpeed = 80.f;
		std::cout << "Key Pressed" << std::endl;
	}

	//In Case this doesn't feel good, we can specify the player speed here.
	/*
	m_playerSpeed = 10 * 2 ^ (level - 1);
	switch (level) {
	case 1:
		m_playerSpeed = 10.f;
		break;
	case 2:
		m_playerSpeed = 20.f;
		break;
	case 3:
		m_playerSpeed = 40.f;
		break;
	}
	*/
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}



