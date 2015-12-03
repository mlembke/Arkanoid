#include "GameObject.h"


GameObject::GameObject() : position_(0.0f, 0.0f), size_(1.0f, 1.0f), velocity_(0.0f, 0.0f), color_(1.0f), rotation_(0.0f), isSolid_(false), destroyed_(false), sprite_()
{

}

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) : position_(position), size_(size), velocity_(velocity), color_(color), rotation_(0.0f), isSolid_(false), destroyed_(false), sprite_(sprite)
{
}

GameObject::~GameObject()
{
}

void GameObject::draw(SpriteRenderer& renderer)
{
	renderer.drawSprite(sprite_, position_, size_, rotation_, color_);
}
