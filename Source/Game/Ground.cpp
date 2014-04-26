#include "Ground.hpp"
#include "Ore.hpp";
#include "../Util/Resources.hpp"
#include "QuadTree.hpp"
#include "World.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <random>

Ground::Ground(Type t) : mType(t), mOre(nullptr), mDug(0)
{
	mSheet = Resources::SpriteSheets["ground.png"];
	mBreakSheet = Resources::SpriteSheets["digging.png"];

	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 1);
	mFlip = dist(rd);
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
		std::random_device rd;
		std::uniform_int_distribution<int> dist(0, 25);
		auto ore = dist(rd) == 0;

		dist = std::uniform_int_distribution<int>(0, 100);
		if (ore)
		{
			Ore::Type oreType = Ore::Max;
			for (int i = Ore::Max-1; i >= 0; --i)
			{
				
				if (dist(rd) < Ore::OreWeight((Ore::Type)i, ((WORLD_HEIGHT * 15.f)+mPosition.y) / (WORLD_HEIGHT*30.f)))
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

void Ground::dig(double dug)
{
	switch (mType)
	{
	case Rock: dug *= 0.5; break;
	}

	mDug = std::min(1.0, mDug + dug);
}

bool Ground::dug() const
{
	return mDug == 1.0;
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
	if (!dug())
		mDug = std::max(0.0, mDug - dt / 10);
}

void Ground::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mSheet.getTexture());
	sprite.setTextureRect(mSheet.getRect(mType, 0));
	sprite.setOrigin(15, 15);
	sprite.setPosition(mPosition);

	if (mFlip)
		sprite.setScale(-1, 1);

	if (dug())
		sprite.setColor(sf::Color(75, 75, 75));

	target.draw(sprite);

	if (mDug > 0 && mDug < 1)
	{
		sprite.setTexture(mBreakSheet.getTexture());
		sprite.setTextureRect(mBreakSheet.getRect(mDug * 6, 0));
		target.draw(sprite);
	}

	if (mOre)
		mOre->draw(target);
}
