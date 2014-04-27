#include "Ladder.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Ladder::Ladder()
{
	mTexture.loadFromFile("ladder.png");
}

Ladder::~Ladder()
{

}

void Ladder::update(double dt)
{

}

void Ladder::setPosition(const sf::Vector2f& pos)
{
	mPosition = pos;
}

sf::Vector2f Ladder::getPosition() const
{
	return mPosition;
}

void Ladder::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mTexture);
	sprite.setOrigin(15, 15);
	sprite.setPosition(mPosition);

	target.draw(sprite);
}

Ladder::LadderItem::LadderItem() : mAmount(0)
{
	mTexture.loadFromFile("ladder.png");
}

void Ladder::LadderItem::draw(sf::RenderTarget& target, const sf::Vector2f& position)
{
	sf::Sprite sprite(mTexture);
	sprite.setOrigin(15, 15);
	sprite.setPosition(position);

	target.draw(sprite);
}