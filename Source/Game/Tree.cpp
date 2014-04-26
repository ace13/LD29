#include "Tree.hpp"
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
		dist = std::uniform_int_distribution<int>(1, 2);
		mStemLength = dist(rd);
		dist = std::uniform_int_distribution<int>(2, 6);
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

sf::Vector2f Tree::getPosition() const
{
	return sf::Vector2f();
}

void Tree::draw(sf::RenderTarget& target)
{
	auto start = target.getView().getCenter() + sf::Vector2f(0, target.getView().getSize().y / 2.f - 15);

	sf::Sprite treeSprite(mSheet.getTexture());
	treeSprite.setOrigin(15, 15);
	treeSprite.setPosition(start);

	for (int i = 0; i < mStemLength; ++i)
	{
		if (i == 0)
			treeSprite.setTextureRect(mSheet.getRect(1, 0));
		else
			treeSprite.setTextureRect(mSheet.getRect(2, 0));

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