#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "Hook.h"
#include <cmath>
#include <random>

int playerX = 0;
int playerY = 0;

b2Vec2 activeProjDir;




PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
	
}


int PhysicsPlayground::ShootHook()
{
	//kill pre existing hook
	if (activeHook != NULL)
	{
		PhysicsBody::m_bodiesToDelete.push_back(activeHook);
	}
	float playerX = ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPositionX();
	float playerY = ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPositionY();

	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);  //Go back and implement triggers the way below does it with pointers instead.

	// To implement: 
	//ECS::AttachComponent<GrappleTrigger>(entity);


	//Sets up the components
	std::string fileName = "BeachBall.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3, 3);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(playerX, playerY, 10));

	ECS::GetComponent<Trigger*>(entity) = new Hook();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	


	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(playerX), float32(playerY));

	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, HOOK, GROUND | ENVIRONMENT, 0.3f); //change to true later



	//--------------------- Vector projectile travels on

	vec3 playerPos = ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition();

	vec2 mouseGL = m_mousePos; //converted world space mouse to GL space mouse

	float Vec1x = -1 * (playerPos.x - mouseGL.x);
	float Vec1y = -1 * (playerPos.y - mouseGL.y);

	float Vec1Magnitude = sqrt(pow(Vec1x, 2.0) + pow(Vec1y, 2.0));
	float projSpeedMult = 10;
	
	activeProjDir = b2Vec2(Vec1x/Vec1Magnitude * projSpeedMult, Vec1y/Vec1Magnitude * projSpeedMult);
	//------------------------------



	tempPhsBody.SetRotationAngleDeg(0);//rotation of the actual sprite
	tempPhsBody.SetGravityScale(0.f);

	ECS::GetComponent<PhysicsBody>(entity).GetBody()->SetLinearVelocity(activeProjDir);
	activeHook = entity; // ref for later use


	ECS::GetComponent<Player>(MainEntities::MainPlayer()).reattachBody(); //mega super duper important when spawning anything at runtime

	return entity;
}



void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
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
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);
		int m = 2;
		vec4 temp = vec4(-80.f*m,80.f*m, -80.f*m, 80.f*m);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}
	
	//Setup the background
	{
		//Creates entity 

		auto entity = ECS::CreateEntity();
		auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Background>(entity);

		//Sets up the components
		std::string fileName = "Background.png";

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 1.f));

		ECS::GetComponent<Background>(entity).InitBackground(fileName, 720, 480, entity);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		background = entity;

	}



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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7676, 2308);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -400.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, ENEMY | OBJECTS | HOOK);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}
	//BEGINNING\\

	/*
	//Setup static Top Platform
	CreateBoxEntity("boxSprite.jpg", 256, 16, 60.f, -20.f);

	
	//Jump over block
	CreateBoxEntity("boxSprite.jpg", 64, 16, 196.f, 10.f, 90);
	//Down - Left
	CreateBoxEntity("boxSprite.jpg", 64, 16, 196.f, -50.f, 90);
	//Down - Bottom
	CreateBoxEntity("boxSprite.jpg", 128, 16, 268.f, -74.f);
	*/

	//Map entity - bottom
	{
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

	}
	//Map entity - top
	{
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
		b2Vec2(-478, 917), b2Vec2(-478, 154), b2Vec2(353, 154), b2Vec2(353, -133), b2Vec2(544, -133),
		b2Vec2(544, 121), b2Vec2(704, 121), b2Vec2(704, -38), b2Vec2(864, -38), b2Vec2(864, 151),
		b2Vec2(800, 151), b2Vec2(800, 665), b2Vec2(1344, 665), b2Vec2(1344, 282), b2Vec2(3169, 282),
		b2Vec2(3169, 55), b2Vec2(3202, 55), b2Vec2(3202, -228), b2Vec2(3616, -228), b2Vec2(3616, 56),
		b2Vec2(3616, 300), b2Vec2(3208, 300), b2Vec2(3208, 322), b2Vec2(3208, 312), b2Vec2(1376, 312),
		b2Vec2(1376, 697), b2Vec2(480, 697), b2Vec2(480, 728), b2Vec2(450, 728), b2Vec2(450, 855),
		b2Vec2(416, 855), b2Vec2(416, 920), b2Vec2(160, 920), b2Vec2(160, 952), b2Vec2(33, 952),
		b2Vec2(33, 919)
		};




		tempPhsBody = PhysicsBody(entity, tempBody, points, false, GROUND, PLAYER | ENEMY | OBJECTS | HOOK);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);

	}

	//Player entity
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<Player>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/CharSpritesheet.png";
		std::string animations = "charAnimations.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 70,70, &ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity), &ECS::GetComponent<CanJump>(entity));


		ECS::GetComponent<Player>(entity).SetScene(this);


		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-3700, -80, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-3700.f), float32(-80.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP, 1.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);

		
		//ECS::GetComponent<Player>(entity).InitPlayer(&ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity), &ECS::GetComponent<CanJump>(entity));
		playerRef = entity;
	}
	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	/*
	ECS::GetComponent<Background>(background).attachTransform(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<Background>(background).attachSprite(&ECS::GetComponent<Sprite>(background));
	*/

}
 

void PhysicsPlayground::Update()
{
	//PrintMouseLocation(m_mousePos);
	ECS::GetComponent<Player>(MainEntities::MainPlayer()).Update();

	ECS::GetComponent<Background>(background).update();

	if (activeHook != NULL)
	{
		ECS::GetComponent<Trigger*>(activeHook)->Update();
	}

	//If the hook is in its "in flight" state, update its movement
	
	//ECS::GetComponent<Hook>(activeHook).Update();
	
	queueDeleteHookCheck();
	queueHookCheck();
	//Scene::AdjustScrollOffset();
}

int PhysicsPlayground::getActiveHook()
{
	return activeHook;
}

void PhysicsPlayground::PrintMouseLocation(vec2 mousePos)
{
	vec3 playerPos = ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition();

	vec2 mouseGL = mousePos; //converted world space mouse to GL space mouse

	float dx = -1*(playerPos.x - mouseGL.x);
	float dy = -1*(playerPos.y - mouseGL.y);

	std::cout << "X: " << dx << "\tY: " << dy << "\n";
}

vec2 PhysicsPlayground::GetMouseLocation()
{
	return m_mousePos;

}

void PhysicsPlayground::MouseMotion(SDL_MouseMotionEvent event)
{
	m_mousePos = Util::ConvertToGL(m_sceneReg, vec2(float(event.x), float(event.y)));
}

void PhysicsPlayground::MousePress(SDL_MouseMotionEvent event)
{
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (Input::GetKeyDown(Key::H))
	{
		queueHook(0);
	}
	if (Input::GetKeyDown(Key::G))
	{
		queueDeleteHook();
	}
	
}

void PhysicsPlayground::KeyboardUp()
{
}



void PhysicsPlayground::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}
}

void PhysicsPlayground::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}

	ImGui::End();
}

void PhysicsPlayground::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void PhysicsPlayground::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
}
