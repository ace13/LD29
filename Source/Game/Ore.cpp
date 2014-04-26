#include "Ore.hpp"
#include "../Util/Resources.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

uint32_t Ore::OreWeight(Type t, float depth)
{
	switch (t)
	{
	case Iron:    return (depth < 0.25f ? 25 : 8);
	case Sulfur:  return (depth < 0.5f ? 8 : 1);
	case Gold:    return 15;
	case Emerald: return (depth > 0.6f ? 8 : 2);
	case Diamond: return (depth > 0.85f ? 25 : 1);
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
