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

		vec4 temp = vec4(-80.f * m, 80.f * m, -80.f * m, 80.f * m);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
	}

	CreateBoxEntity("TitleScreen.png", 576, 325, 0.f, 0.f, 0, 2);
	CreateBoxEntity("start.png", 75, 49.875, -135.f, 0.f, 0, 3.f);
	CreateBoxEntity("credits.png", 75, 49.875, -135.f, -49.875, 0, 3.f);
	CreateBoxEntity("exit.png", 75, 49.875, -135.f, -99.75, 0, 3.f);
}

void TitleScreen::Update()
{
	/*
	float dx = m_mousePos.x;
	float dy = m_mousePos.y;
	std::cout << "X: " << dx << "   Y: " << dy << std::endl;
	*/
}

void TitleScreen::MouseClick(SDL_MouseButtonEvent evnt)
{
	float dx = m_mousePos.x;
	float dy = m_mousePos.y;
	ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));

	if (ImGui::GetIO().MouseDown[1] && (dx >= -165.f && dx <= -105.f) && (dy >= -10 && dy <= 10))
	{
		std::cout << "Starting game!" << std::endl;

		sceneNum = 1;
	}
	if (ImGui::GetIO().MouseDown[1] && (dx >= -170.f && dx <= -100.f) && (dy >= -60 && dy <= -40))
	{
		std::cout << "Credits!" << std::endl;

	}
	if (ImGui::GetIO().MouseDown[1] && (dx >= -155.f && dx <= -95.f) && (dy >= -110 && dy <= -90))
	{
		std::cout << "Thank you for playing!" << std::endl;
		exit(0);
	}
}

void TitleScreen::MouseMotion(SDL_MouseMotionEvent event)
{
	m_mousePos = Util::ConvertToGL(m_sceneReg, vec2(float(event.x), float(event.y)));
}

void TitleScreen::KeyboardDown()
{

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	/* I am trying to figure this out so it's relevant with player.cpp
	if (Input::GetKeyDown(Key::LeftControl))
	{
		queueAtk();
	}
	*/
}