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

	//Mouse Input
	void MouseClick(SDL_MouseButtonEvent evnt);

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	int ShootHook();
	

	
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

	std::vector <unsigned int> EnemyEnts = {};

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

	int Attack();
	void queueAtk()
	{
		m_attackTimer = true;
	}

	void startAtk()
	{
		if (m_attackTimer)
		{
			Attack();
			m_attackTimer = false;
			m_attackDelete = true;
		}
	}

	void deleteAtk()
	{
		if (m_attackDelete)
		{
			if (activeATK != NULL)
			{
				PhysicsBody::m_bodiesToDelete.push_back(activeATK);
				activeATK = NULL;
			}
			m_attackDelete = false;
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

	bool m_attackDelete = false;
	bool m_attackTimer = false;
	int activeATK = NULL;

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int background = 1;

	unsigned int backgroundID;

	int puzzleWall1;
	int puzzleWall2;
};
