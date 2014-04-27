#include "Wood.hpp"
#include "../../Util/Resources.hpp"
#include <SFML/Graphics/Sprite.hpp>

Wood::Wood() : mAmount(0)
{
	mSheet = Resources::SpriteSheets["resources.png"];
}

void Wood::addAmount(float amount)
{
	mAmount += amount;
}

void Wood::draw(sf::RenderTarget& target, const sf::Vector2f& position)
{
	sf::Sprite sprite(mSheet.getTexture(), mSheet.getRect(0, 0));
	sprite.setOrigin(15, 15);
	sprite.setPosition(position);

	target.draw(sprite);
}