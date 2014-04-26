#include "Building.hpp"

#include "../Util/Resources.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Building::Building()
{
	mSheet = Resources::SpriteSheets["building.png"];
}

Building::~Building()
{

}

void Building::update(double dt)
{

}

void Building::setPosition(const sf::Vector2f& pos)
{
	mPosition = pos;
}

sf::Vector2f Building::getPosition() const
{
	return mPosition;
}

void Building::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mSheet.getTexture());
	sprite.setTextureRect(mSheet.getRect(0, 1));
	sprite.setOrigin(15, 15);

	sprite.setPosition(mPosition);
	target.draw(sprite);

	sprite.setTextureRect(mSheet.getRect(4, 0));
	sprite.move(5, 0);
	target.draw(sprite);
	sprite.move(-5, 0);

	sprite.setTextureRect(mSheet.getRect(1, 1));
	sprite.move(30, 0);
	target.draw(sprite);

	sprite.setTextureRect(mSheet.getRect(3, 1));
	sprite.move(-5, 0);
	target.draw(sprite);
	sprite.move(5, 0);

	sprite.setTextureRect(mSheet.getRect(1, 0));
	sprite.move(0, -30);
	target.draw(sprite);

	sprite.setTextureRect(mSheet.getRect(0, 0));
	sprite.move(-30, 0);
	target.draw(sprite);
}
