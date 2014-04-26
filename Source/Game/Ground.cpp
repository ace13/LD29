#include "Ground.hpp"
#include "Ore.hpp";
#include "../Util/Resources.hpp"
#include "QuadTree.hpp"
#include "World.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <random>

Ground::Ground(Type t) : mType(t), mOre(nullptr)
{
	mSheet = Resources::SpriteSheets["ground.png"];

	mFlip = std::uniform_int_distribution<int>(0, 1)(std::random_device());
}

Ground::~Ground()
{
	if (mOre)
		delete mOre;
}

void Ground::genOre()
{
	if (mType == Grass)
	{

	}
	else
	{
		auto ore = std::uniform_int_distribution<int>(0, 25)(std::random_device()) == 0;

		if (ore)
		{
			Ore::Type oreType = Ore::Max;
			for (int i = Ore::Max-1; i >= 0; --i)
			{
				if (std::uniform_int_distribution<int>(0, 100)(std::random_device()) < Ore::OreWeight((Ore::Type)i, ((WORLD_HEIGHT * 15.f)+mPosition.y) / (WORLD_HEIGHT*30.f)))
				{
					oreType = (Ore::Type)i;
					break;
				}
			}

			if (oreType == Ore::Max)
				return;

			mOre = new Ore(this, (Ore::Type)oreType);
		}
	}
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

void Ground::update(double dt)
{

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

	if (mOre)
		mOre->draw(target);
}
