#include "TitleScreen.h"
#include "Utilities.h"

TitleScreen::TitleScreen(std::string name) 
	: Scene(name)
{
	//title screen player ain't doin anythin
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);


}

void TitleScreen::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	//m_physicsWorld = new b2World(m_gravity);
	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		int m = 2;

		vec4 temp = vec4(-80.f * m, 80.f * m, -80.f * m, 80.f * m);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
	}

	CreateBoxEntity("TitleScreen.png", 576, 325, 0, 0, false);
	CreateDecoration("start.png", 75.f, 49.875, -135.f, 0.f, 3.f);
	CreateDecoration("credits.png", 75.f, 49.875, -135.f, -49.875, 3.f);
	CreateDecoration("exit.png", 75.f, 49.875, -135.f, -99.75, 3.f);

	//Loading Screen
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainLoading(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "loading.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 576, 325);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(2000.f, 2000.f, 10.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2000.f), float32(2000.f));
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - 0.f), float(tempSpr.GetHeight() - 0.f), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HOOK);

		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Credits Screen
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCredits(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "credit.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 576, 325);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(2000.f, 2000.f, 10.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2000.f), float32(2000.f));
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - 0.f), float(tempSpr.GetHeight() - 0.f), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HOOK);

		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	Sound.Play();
	Sound.SetVolume(0.2);

}

void TitleScreen::Update()
{
	auto& loading = ECS::GetComponent<PhysicsBody>(MainEntities::MainLoading());
	auto& credits = ECS::GetComponent<PhysicsBody>(MainEntities::MainCredits());
	ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));

	fmod.Update();
	

	if (play)
	{
		if (timer > 0)
		{
			loading.SetPosition(b2Vec2(0, 0), true);
			timer -= Timer::deltaTime;
		}
		else
		{
			setScene = true;
		}
	}
		
	if (setScene)
	{
		sceneNum = 1;
		Sound.Mute();
	}

	if (displayCredits)
	{
		credits.SetPosition(b2Vec2(0, 0), true);
		if (inputCredits)
		{
			if (ImGui::GetIO().MouseDown[2])
			{
				credits.SetPosition(b2Vec2(2000, 2000), true);
				displayCredits = false;
				inputCredits = false;
				menu = true;
			}
		}
	}

	/*
	float dx = m_mousePos.x;
	float dy = m_mousePos.y;
	//std::cout << "X: " << dx << "   Y: " << dy << std::endl;
	*/
}

void TitleScreen::MouseClick(SDL_MouseButtonEvent evnt)
{
	float dx = m_mousePos.x;
	float dy = m_mousePos.y;
	ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
	//ToneFire::CoreSound Select("boing2.wav");

	if (menu)
	{
		if (ImGui::GetIO().MouseDown[1] && (dx >= -165.f && dx <= -105.f) && (dy >= -10 && dy <= 10))
		{
			//Select.Play();
			//std::cout << "Starting game!" << std::endl;
			timer = 1.f;
			menu = false;
			play = true;
		}
		if (ImGui::GetIO().MouseDown[1] && (dx >= -170.f && dx <= -100.f) && (dy >= -60 && dy <= -40))
		{
			//Select.Play();
			//std::cout << "Credits!" << std::endl;
			menu = false;
			displayCredits = true;
			inputCredits = true;
		}
		if (ImGui::GetIO().MouseDown[1] && (dx >= -155.f && dx <= -95.f) && (dy >= -110 && dy <= -90))
		{
			//Select.Play();
			//std::cout << "Thank you for playing!" << std::endl;
			exit(0);
		}
	}
}

void TitleScreen::MouseMotion(SDL_MouseMotionEvent event)
{
	m_mousePos = Util::ConvertToGL(m_sceneReg, vec2(float(event.x), float(event.y)));
}