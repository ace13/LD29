#include "Sapling.hpp"
#include <SFML/Graphics/Sprite.hpp>

Sapling::Sapling() : mAmount(5)
{
	mTexture.loadFromFile("sapling.png");
}

void Sapling::draw(sf::RenderTarget& target, const sf::Vector2f& position)
{
	sf::Sprite sprite(mTexture);
	sprite.setOrigin(15, 15);
	sprite.setPosition(position);

	target.draw(sprite);
}