#include "Background.h"

Background::Background()
{
}
/*
Background::Background(std::string& fileName, int width, int height, Sprite* sprite, Transform* transform)
{
	InitBackground(fileName, width, height, sprite, transform);
}

void Background::InitBackground(std::string& fileName, int width, int height, Sprite* sprite, Transform* transform, unsigned int ID)
{
    //Store references to the components
    m_sprite = sprite;
    m_transform = transform;
    m_sprite->LoadSprite(fileName, width, height, false);
}
*/
Background::Background(std::string& fileName, int width, int height, unsigned int ID)
{
    InitBackground(fileName, width, height, ID);
}

void Background::InitBackground(std::string& fileName, int width, int height, unsigned int ID)
{
    
    //Store references to the components
    bgID = ID;

    auto& sprite = ECS::GetComponent<Sprite>(bgID);

    sprite.LoadSprite(fileName, width, height, false);
}

void Background::update()
{
    auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
    auto& trans = ECS::GetComponent<Transform>(bgID);
    trans.SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 1.f));

}

/*
void Background::attachTransform(Transform* transform)
{
    m_transform = transform;
}

void Background::attachSprite(Sprite* sprite)
{
    m_sprite = sprite;
}
*/