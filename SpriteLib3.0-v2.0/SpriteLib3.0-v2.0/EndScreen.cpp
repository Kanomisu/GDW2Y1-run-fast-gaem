#include "EndScreen.h"
#include "Utilities.h"

EndScreen::EndScreen(std::string name) : Scene(name)
{
	//title screen player ain't doin anythin
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void EndScreen::InitScene(float windowWidth, float windowHeight)
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

	CreateBoxEntity("end.png", 576, 325, 0, 0, 0, 2);

	//Tone Fire !!!!!!!!!!!!!!!!
	Sound.Play();
	Sound.SetVolume(0.1);
}

void EndScreen::Update()
{
	fmod.Update();
}

void EndScreen::MouseClick(SDL_MouseButtonEvent evnt)
{
	ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
	ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));

	if (ImGui::GetIO().MouseDown[1] || ImGui::GetIO().MouseDown[2])
	{
		exit(0);
		//everything breaks
	}
}

void EndScreen::MouseMotion(SDL_MouseMotionEvent event)
{
	m_mousePos = Util::ConvertToGL(m_sceneReg, vec2(float(event.x), float(event.y)));
}
