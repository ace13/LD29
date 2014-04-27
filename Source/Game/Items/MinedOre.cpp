#include "MinedOre.hpp"
#include "../../Util/Resources.hpp"
#include <SFML/Graphics/Sprite.hpp>

MinedOre::MinedOre(Ore::Type t) : mType(t), mAmount(0)
{
	mSheet = Resources::SpriteSheets["resources.png"];
}

std::string MinedOre::getName() const
{
	switch (mType)
	{
	case Ore::Iron: return "Iron"; break;
	case Ore::Gold: return "Gold"; break;
	case Ore::Sulfur: return "Sulfur"; break;
	case Ore::Emerald: return "Emerald"; break;
	case Ore::Diamond: return "Diamond"; break;
	}
}

void MinedOre::draw(sf::RenderTarget& target, const sf::Vector2f& position)
{
	sf::Sprite sprite(mSheet.getTexture(), mSheet.getRect(1 + mType, mType / 5));
	sprite.setOrigin(15, 15);
	sprite.setPosition(position);

	target.draw(sprite);
}