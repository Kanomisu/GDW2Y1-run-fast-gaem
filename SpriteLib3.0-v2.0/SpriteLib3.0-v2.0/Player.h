#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "BackEnd.h"

//#define TOPDOWN

enum AnimEnums
{
	IDLELEFT,
	IDLERIGHT,
	
	//Only in Top down
#ifdef TOPDOWN
	IDLEUP,
	IDLEDOWN,
#endif

	WALKLEFT,
	WALKRIGHT,

	//Only in Top down
#ifdef TOPDOWN
	WALKUP,
	WALKDOWN,
#endif
	
	ATTACKLEFT,
	ATTACKRIGHT,

	JUMPLEFT,
	JUMPRIGHT,

	DASHLEFT,
	DASHRIGHT,
	//Only in Top down
#ifdef TOPDOWN
	ATTACKUP,
	ATTACKDOWN
#endif
};

enum AnimTypes
{
#ifdef TOPDOWN
	IDLE = 0,
	WALK = 4,
	ATTACK = 8
#endif
#ifndef TOPDOWN
	IDLE = 0,
	WALK = 2,
	DASH = 4,
	JUMP = 6,
	ATTACK = 8
#endif
};

enum AnimDir
{
	LEFT,
	RIGHT,
	//Only in Top Down
#ifdef TOPDOWN
	UP,
	DOWN
#endif
};
class PhysicsPlayground;
class Player
{
public:
	Player();
	Player(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr, CanJump* jump = nullptr);

	void InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr, CanJump* jump = nullptr);
	
	//Alternative Init when no animations are implemented
	void InitPlayer(Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr, CanJump* jump = nullptr);

	void Update();
	void MovementUpdate();
	void AnimationUpdate();
	void PlayerSpeedLevel();
	void damage();

	void reattachBody()
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		m_physBody = &ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	}
	
	void SetScene(PhysicsPlayground* scene);

	PhysicsPlayground* m_currScene;

	//Are you currently attacking?????
	bool m_attacking = false;
private:

	void SetActiveAnimation(int anim);

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;

	//Are you currently jumping?????
	bool m_jumping = false;
	//Have we locked the player from moving during this animation?
	bool m_locked = false;
	bool m_dashing = false;
	//For setting how long the player is in the dash.
	float m_dashTime = 1.f;
	//For keeping track of how long a dash is
	float m_dashTimeDelta = 0.f;
	//How fast the player moved while dashing
	float m_dashSpeed = 100.f;

	float m_jumpHeight = 80000.f;


	//Player's speed.
	float m_playerSpeed = 10.f;
	//Player's previous momentum

	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;
	//A refernce to the player's "CanJump" Component
	CanJump* m_canJump = nullptr;


	//Physics importance
	//A reference to our physics body
	PhysicsBody* m_physBody = nullptr;
	//Does this player have physics?
	bool m_hasPhysics = false;

	//Default animation direction (feel free to change this to suit your game. If you're making a side-scroller, left or right would be better
	AnimDir m_facing = RIGHT;
};

#endif // !__PLAYER_H__