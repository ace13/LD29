#include "Ore.hpp"
#include "../Util/Resources.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

uint32_t Ore::OreWeight(Type t)
{
	switch (t)
	{
	case Iron:    return 50;
	case Sulfur:  return 20;
	case Gold:    return 5;
	case Emerald: return 2;
	case Diamond: return 1;
	}
}

Ore::Ore(Ground* g, Type t) : mGround(g), mType(t)
{
	mSheet = Resources::SpriteSheets["ores.png"];
}

Ore::~Ore()
{

}

void Ore::update(double dt)
{

}

void Ore::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mSheet.getTexture());
	sprite.setTextureRect(mSheet.getRect(mType % 5, mType / 5));
	sprite.setOrigin(15, 15);
	sprite.setPosition(mGround->getPosition());

	target.draw(sprite);
}
