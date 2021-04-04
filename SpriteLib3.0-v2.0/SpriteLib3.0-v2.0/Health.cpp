#include "Health.h"
#include "Utilities.h"
//#include "ECS.h"
Health::Health()
{
}

Health::Health(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID)
{
	InitHealth(fileName, animationJSON, width, height, ID);
}

void Health::InitHealth(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID)
{
	hID = ID;
	auto& sprite = ECS::GetComponent<Sprite>(hID);
	auto& animController = ECS::GetComponent<AnimationController>(hID);
	animController.InitUVs(fileName);
	sprite.LoadSprite(fileName, width, height, true, &animController);
	animController.SetVAO(sprite.GetVAO());
	animController.SetTextureSize(sprite.GetTextureSize());

	nlohmann::json animations = File::LoadJSON(animationJSON);

	animController.AddAnimation(animations["100%"].get<Animation>());
	animController.AddAnimation(animations["66%"].get<Animation>());
	animController.AddAnimation(animations["33%"].get<Animation>());
	animController.AddAnimation(animations["0%"].get<Animation>());

	animController.SetActiveAnim(0);


}

void Health::update(unsigned int pID)
{
	auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	auto& trans = ECS::GetComponent<Transform>(hID);
	trans.SetPosition(vec3(Cam.GetPosition().x - 180, Cam.GetPosition().y + 100, 20.f));

	int health = ECS::GetComponent<Player>(pID).GetHealth();
	animUpdate(health);
}

void Health::animUpdate(int hp)
{
	int activeAnimation = 0;

	switch (hp)
	{
	case 0:
		activeAnimation = 3;
		break;
	case 1:
		activeAnimation = 2;
		break;
	case 2:
		activeAnimation = 1;
		break;
	case 3:
		activeAnimation = 0;
		break;
	default:
		break;
	}
	auto& animController = ECS::GetComponent<AnimationController>(hID);
	animController.SetActiveAnim(activeAnimation);
}