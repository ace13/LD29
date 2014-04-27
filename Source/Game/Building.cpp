#include "Building.hpp"

#include "../Util/Resources.hpp"
#include "../Util/ShapeDraw.hpp"
#include "../Util/FontFinder.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

Building::Building() : mDoorAnim(0), mMenuAnim(0), mSelectedEntry(0)
{
	mSheet = Resources::SpriteSheets["building.png"];
	mMenuSheet = Resources::SpriteSheets["buildingmenu.png"];
	mFont = FontFinder::findDefaultFont();
}

Building::~Building()
{

}

void Building::update(double dt)
{
	mDoorAnim = std::max(0.0, mDoorAnim - dt);
	mMenuAnim = mMenuAnim + (mSelectedEntry - mMenuAnim) * dt * 2;
}

void Building::doorOpen()
{
	mDoorAnim = 1;
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
	sprite.move(-30, 0);
	target.draw(sprite);

	sprite.setTextureRect(mSheet.getRect(4, 0));
	sprite.move(5, 0);
	target.draw(sprite);
	sprite.move(-5, 0);

	sprite.setTextureRect(mSheet.getRect(1, 1));
	sprite.move(30, 0);
	target.draw(sprite);

	if (mDoorAnim < 0.5)
		sprite.setTextureRect(mSheet.getRect(3, 1));
	else
		sprite.setTextureRect(mSheet.getRect(4, 1));
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

void Building::drawMenu(sf::RenderTarget& target)
{
	Shapes::Arc arc;
	arc.setStartAngle(0);
	arc.setEndAngle(360);
	arc.setRadius(45);
	arc.setLineThickness(4);
	//arc.setColor(sf::Color(75, 75, 75));

	arc.setOrigin(45, 45);
	arc.setPosition(mPosition - sf::Vector2f(15, 10));

	target.draw(arc);

	sf::CircleShape entry(16);
	entry.setOrigin(16, 16);
	entry.setFillColor(sf::Color::White);
	entry.setOutlineColor(sf::Color::Black);
	entry.setOutlineThickness(2.f);

	const std::string entryNames[6] = { "Exit", "Store", "Retrieve", "Crafting", "<Insert feature here>", "<Insert another (more useful) feature here, like the support of long strings>" };

	sf::Text entryName("<Entry Name>", mFont, 18U);
	entryName.setColor(sf::Color::Black);
	{
		auto rect = entryName.getLocalBounds();
		entryName.setOrigin(-25, rect.height / 2.f);
	}

	sf::Sprite entrySprite(mMenuSheet.getTexture());
	entrySprite.setTextureRect(mMenuSheet.getRect(0, 0));
	entrySprite.setOrigin(15, 15);

	for (int i = 0; i < 6; ++i)
	{
		float rad = (i - mMenuAnim) * ((3.1415f * 2) / 6);

		entry.setPosition(mPosition - sf::Vector2f(15, 10) + sf::Vector2f(cos(rad) * 45, sin(rad) * 45));

		target.draw(entry);

		entrySprite.setTextureRect(mMenuSheet.getRect(i, 0));
		entrySprite.setPosition(entry.getPosition());
		//entrySprite.setRotation(rad * (180 / 3.1415f));

		target.draw(entrySprite);

		if (i == mSelectedEntry)
		{
			entryName.setColor(sf::Color::Black);
			entryName.move(1.5, 1.5);

			entryName.setString(entryNames[i]);
			entryName.setPosition(entry.getPosition());
			entryName.setRotation(rad * (180 / 3.1415f));
			target.draw(entryName);

			entryName.move(-1.5, -1.5);
			entryName.setColor(sf::Color::White);

			target.draw(entryName);
		}
	}
}
