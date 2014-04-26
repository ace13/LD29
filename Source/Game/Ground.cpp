#include "Ground.hpp"
#include "../Util/Resources.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <random>

Ground::Ground() : mType(Grass)
{
	mSheet = Resources::SpriteSheets["ground.png"];

	mFlip = std::uniform_int_distribution<int>(0, 1)(std::random_device());
}

Ground::~Ground()
{

}

void Ground::setType(Type t)
{
	mType = t;
}

void Ground::setPosition(const sf::Vector2f& pos)
{
	mPosition = pos;
}

sf::Vector2f Ground::getPosition() const
{
	return mPosition;
}

void Ground::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mSheet.getTexture());
	sprite.setTextureRect(mSheet.getRect(mType, 0));
	sprite.setOrigin(15, 15);
	sprite.setPosition(mPosition);

	if (mFlip)
		sprite.setScale(-1, 1);

	target.draw(sprite);
}
