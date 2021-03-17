#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>

class PhysicsPlayground : public Scene 
{
	
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();


	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	int ShootHook();
	
	void SpawnEnemy(float spawnX, float spawnY, float roamA, float roamB) {
		//Enemy entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		//ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Enemy>(entity);

		//Sets up the components
		std::string fileName = "Enemy.png";
		//Enemy is oversized (should be size of hitbox). Plz fix ricky
		//std::string animations = "charAnimations.json";
		ECS::GetComponent<Enemy>(entity).Init(&ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<Transform>(entity), &ECS::GetComponent<PhysicsBody>(entity), roamA, roamB, entity);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(spawnX, spawnY, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(spawnX), float32(spawnY));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), float((tempSpr.GetWidth() - shrinkX) / 2.f), vec2(0.f, 0.f), false, ENEMY, ENVIRONMENT | PLAYER | OBJECTS | PICKUP | GROUND, 1.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);

		enemyEnts.push_back(entity);
	}
	
	void queueHook(float rotationDeg = 0.0)
	{
		m_hookQueued = true;
		m_hookRotation = rotationDeg;
	}
	void queueHookCheck()
	{
		if (m_hookQueued)
		{
			ShootHook();
			m_hookQueued = false;
		}
	}

	int getActiveHook();

	void queueDeleteHook()
	{
		m_hookDeleteQueued = true;
	}

	void queueDeleteHookCheck()
	{
		if (m_hookDeleteQueued)
		{
			if (activeHook != NULL)
			{
				PhysicsBody::m_bodiesToDelete.push_back(activeHook);
				activeHook = NULL;
			}
			m_hookDeleteQueued = false;
		}
	}

	void PrintMouseLocation(vec2 mousePos);
	vec2 GetMouseLocation();
	void MouseMotion(SDL_MouseMotionEvent event);
	void MousePress(SDL_MouseMotionEvent event);


protected:
	bool m_hookDeleteQueued = false;
	bool m_hookQueued = false;
	float m_hookRotation = 0.0;

	int playerRef;
	int activeHook = NULL;

	vec2 m_mousePos = vec2(0.f, 0.f);

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	std::vector <unsigned int> enemyEnts = {};

	int background = 1;

	int puzzleWall1;
	int puzzleWall2;
};
