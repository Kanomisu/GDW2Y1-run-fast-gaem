#include "TestScene.h"
#include "Utilities.h"
TestScene::TestScene(std::string name) : Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	
}

void TestScene::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	/*
	EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	EffectManager::CreateEffect(EffectType::Sepia, windowWidth, windowHeight);
	*/


	//Setup MainCamera Entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		int m = 2;

		vec4 temp = vec4(-80.f*m, 80.f*m, -80.f*m, 80.f*m);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);


		
	}

	//Map entity
	/*{
		auto entity = ECS::CreateEntity();

		//Add components
		//ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		//ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "map.png";
		




		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		//ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);


		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);


		//points lul
		std::vector<b2Vec2> points = {
        b2Vec2(-3805, -103), b2Vec2(-3520, -103), b2Vec2(-3520, -10), b2Vec2(-3456, -10), b2Vec2(-3456, -102),
        b2Vec2(-3295, -182), b2Vec2(-3295, -40), b2Vec2(-2944, -40), b2Vec2(-2944, -104), b2Vec2(-2880, -104),
        b2Vec2(-2880, -168), b2Vec2(-2817, -168), b2Vec2(-2817, -232), b2Vec2(-2752, -232), b2Vec2(-2752, -297),
        b2Vec2(-2687, -297), b2Vec2(-2687, -40), b2Vec2(-2493, -40), b2Vec2(-2236, 88), b2Vec2(-2111, 88),
        b2Vec2(-2111, 183), b2Vec2(-1982, 183), b2Vec2(-1982, 342), b2Vec2(-1990, 358), b2Vec2(-2000, 358),
        b2Vec2(-2001, 364), b2Vec2(-1921, 408), b2Vec2(-1729, 408), b2Vec2(-1729, 312), b2Vec2(-1530, 216),
        b2Vec2(-1311, 216), b2Vec2(-1311, 439), b2Vec2(-1317, 453), b2Vec2(-1328, 453), b2Vec2(-1328, 461),
        b2Vec2(-1248, 504), b2Vec2(-1052, 504), b2Vec2(-670, 696), b2Vec2(-607, 696), b2Vec2(-607, 570),
        b2Vec2(-639, 570), b2Vec2(-639, 475), b2Vec2(-671, 475), b2Vec2(-671, 442), b2Vec2(-704, 442),
        b2Vec2(-704, 411), b2Vec2(-737, 411), b2Vec2(-737, 185), b2Vec2(-704, 185), b2Vec2(-704, 93),
        b2Vec2(-735, 93), b2Vec2(-735, 58), b2Vec2(-769, 58), b2Vec2(-769, 26), b2Vec2(-800, 26),
        b2Vec2(-800, -101), b2Vec2(-830, -101), b2Vec2(-830, -360), b2Vec2(-702, -295), b2Vec2(-538, -295),
        b2Vec2(-347, -392), b2Vec2(352, -392), b2Vec2(352, -201), b2Vec2(641, -201), b2Vec2(641, -647),
        b2Vec2(896, -647), b2Vec2(896, -201), b2Vec2(1057, -201), b2Vec2(1394, -368), b2Vec2(1394, -375),
        b2Vec2(1383, -375), b2Vec2(1376, -387), b2Vec2(1376, -421), b2Vec2(1279, -421), b2Vec2(1279, -452),
        b2Vec2(1250, -452), b2Vec2(1250, -484), b2Vec2(1215, -484), b2Vec2(1215, -613), b2Vec2(1601, -613),
        b2Vec2(1601, -330), b2Vec2(1727, -330), b2Vec2(1727, -646), b2Vec2(1761, -646), b2Vec2(1761, -710),
        b2Vec2(2079, -710), b2Vec2(2079, -359), b2Vec2(2212, -359), b2Vec2(2405, -456), b2Vec2(2591, -456),
        b2Vec2(2591, -773), b2Vec2(3169, -773), b2Vec2(3169, -360), b2Vec2(3617, -360), b2Vec2(3617, -939),
        b2Vec2(-1267, -939), b2Vec2(-1267, -675), b2Vec2(-1354, -675), b2Vec2(-1354, -490), b2Vec2(-1354, -231),
        b2Vec2(-2080, -231), b2Vec2(-2080, -357), b2Vec2(-3807, -357)
		};




		tempPhsBody = PhysicsBody(entity, tempBody, points, false, GROUND, PLAYER | ENEMY | OBJECTS | HOOK);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);

	}*/
	//Testing Sizing
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "map.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10775, 2308);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1579.5f, -10.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1579.5f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, ENEMY | OBJECTS | HOOK);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}
}

void TestScene::Update()
{


}

void TestScene::KeyboardHold()
{
	auto& cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	float speed = 500.0;

	if (Input::GetKey(Key::LeftShift))
	{
		speed *= 3;
	}

	if (Input::GetKey(Key::D))
	{
		cam.SetPosition(cam.GetPosition() + vec3(Timer::deltaTime * speed, 0, 0));
	}
	if (Input::GetKey(Key::A))
	{
		cam.SetPosition(cam.GetPosition() + vec3(-Timer::deltaTime * speed, 0, 0));
	}
	if (Input::GetKey(Key::S))
	{
		cam.SetPosition(cam.GetPosition() + vec3(0, -Timer::deltaTime * speed, 0));
	}
	if (Input::GetKey(Key::W))
	{
		cam.SetPosition(cam.GetPosition() + vec3(0, Timer::deltaTime * speed, 0));
	}
}

void TestScene::KeyboardDown()
{
	if (Input::GetKeyDown(Key::Space))
	{
		std::cout << "std::vector<b2Vec2> points = {\n\t";
		for (int i = 0; i < positions.size(); i++)
		{
			std::cout << "b2Vec2(" << positions[i].x << ", " << positions[i].y;
			if (i == positions.size() - 1)
			{
				std::cout << ")\n";
			}
			else if (i%5 ==  4)
			{
				std::cout << "),\n\t";
			}
			else
			{
				std::cout << "), ";
			}
		}
		std::cout << "};";
	}


}

void TestScene::KeyboardUp()
{
}



void TestScene::MouseClick(SDL_MouseButtonEvent evnt)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{

		vec2 pos = Util::ConvertToGL(m_sceneReg, vec2(float(evnt.x), float(evnt.y)));

		if (positions.size() > 0 && !Input::GetKey(Key::R))
		{
			b2Vec2 prePos = positions[positions.size() - 1];
			if (fabsf(pos.x - prePos.x) > fabsf(pos.y - prePos.y))
			{
				pos.y = prePos.y;
			}
			else
			{
				pos.x = prePos.x;
			}
		}
		positions.push_back(b2Vec2(int(pos.x), int(pos.y)));

		std::cout << positions.size() - 1 << ": [" << int(pos.x) << ", " << int(pos.y) << "]\n";
	}
}




