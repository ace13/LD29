#include "Tree.hpp"
#include "QuadTree.hpp"
#include "../Util/Resources.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <random>

Tree::Tree()
{
	mSheet = Resources::SpriteSheets["tree.png"];

	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 1);

	bool spruce = dist(rd);

	if (spruce)
	{
		mStemLength = 1;
		dist = std::uniform_int_distribution<int>(3, 4);
		mCrownLength = dist(rd);
	}
	else // Fir
	{
		dist = std::uniform_int_distribution<int>(3, 6);
		mStemLength = dist(rd);
		dist = std::uniform_int_distribution<int>(2, 4);
		mCrownLength = dist(rd);
	}
}

Tree::~Tree()
{

}

void Tree::chop()
{
	mQTLeaf->removeActor(this);
	delete this;
}

void Tree::update(double dt)
{

}

void Tree::setPosition(const sf::Vector2f& pos)
{
	mPos = pos;
}

sf::Vector2f Tree::getPosition() const
{
	return mPos;
}

void Tree::draw(sf::RenderTarget& target)
{
	sf::Sprite treeSprite(mSheet.getTexture());
	treeSprite.setOrigin(15, 15);
	treeSprite.setPosition(mPos);

	for (int i = 0; i < mStemLength; ++i)
	{
		if (i == 0)
			treeSprite.setTextureRect(mSheet.getRect(1, 0));
		else
			treeSprite.setTextureRect(mSheet.getRect(2, 0));

		if (i > 0 && i % 3 == 0)
			treeSprite.setScale(-1, 1);
		else
			treeSprite.setScale(1, 1);

		target.draw(treeSprite);
		treeSprite.move(0, -30);
	}
	for (int i = 0; i < mCrownLength; ++i)
	{
		if (i == 0)
			treeSprite.setTextureRect(mSheet.getRect(3, 0));
		else if (i == mCrownLength - 1)
			treeSprite.setTextureRect(mSheet.getRect(5, 0));
		else
			treeSprite.setTextureRect(mSheet.getRect(4, 0));

		target.draw(treeSprite);
		treeSprite.move(0, -30);
	}
}