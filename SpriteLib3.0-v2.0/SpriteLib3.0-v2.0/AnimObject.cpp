#include "AnimObject.h"
#include "Utilities.h"

AnimObject::AnimObject()
{
}

AnimObject::AnimObject(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID)
{
	InitObject(fileName, animationJSON, width, height, ID);
}

void AnimObject::InitObject(std::string& fileName, std::string& animationJSON, int width, int height, unsigned int ID)
{
	aID = ID;
	auto& sprite = ECS::GetComponent<Sprite>(aID);
	auto& animController = ECS::GetComponent<AnimationController>(aID);
	animController.InitUVs(fileName);
	sprite.LoadSprite(fileName, width, height, true, &animController);
	animController.SetVAO(sprite.GetVAO());
	animController.SetTextureSize(sprite.GetTextureSize());

	nlohmann::json animations = File::LoadJSON(animationJSON);

	animController.AddAnimation(animations["Idle"].get<Animation>());
	animController.SetActiveAnim(0);

}
