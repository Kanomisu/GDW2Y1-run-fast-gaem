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

		vec4 temp = vec4(-80.f, 80.f, -80.f, 80.f);
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
		std::string fileName = "spritesheets/CharSpritesheet.png";
		std::string animations = "charAnimations.json";
		




		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		//ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);


		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);


		//points lul





		tempPhsBody = PhysicsBody(entity, tempBody, points, false, GROUND, PLAYER | ENEMY | OBJECTS | HOOK);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);

	}*/
}

void TestScene::Update()
{


}

void TestScene::KeyboardHold()
{
	auto& cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	float speed = 5.0;

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




