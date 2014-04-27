#include "Building.hpp"

#include "../Util/Resources.hpp"
#include "../Util/ShapeDraw.hpp"
#include "../Util/FontFinder.hpp"
#include "Item.hpp"
#include "Recipes.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iomanip>
#include <sstream>

Building::Building() : mInventory(-1, 50), mDoorAnim(0), mMenuAnim(0), mSelectedEntry(0), mSelectedInventorySlot(0), mCurMenu(Menu_Main)
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

void Building::drawMenu(sf::RenderTarget& target, MenuType type)
{
	if (type == Menu_Main)
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

		const std::string entryNames[6] = { "Exit", "Inventory", "Crafting", "<Insert feature here>", "<Insert feature here>", "<Insert another (more useful) feature here, like the support of long strings>" };

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
	else if (type == Menu_Inventory)
	{
		sf::RectangleShape background(sf::Vector2f(30.f * 10 + 10, 30.f * 5 + 10));
		background.setOrigin(0, 0); // 15.f * 5 + 5);
		background.setFillColor(sf::Color(75, 75, 75, 200));
		background.setOutlineThickness(1.f);
		background.setOutlineColor(sf::Color::Black);

		background.setPosition(mPosition + sf::Vector2f(35, -(30.f * 6 + 10)));

		target.draw(background);

		std::ostringstream oss;
		
		if (mInventory.getItem(mSelectedInventorySlot))
		{
			auto item = mInventory.getItem(mSelectedInventorySlot);
			oss << item->getName() << " (" << std::setprecision(2) << item->getWeight() << " kg) - ";
		}

		oss << "Total: " << std::setprecision(2) << mInventory.getCurWeight() << " kg";

		sf::Text infoText(oss.str(), mFont, 16U);
		infoText.setPosition(background.getPosition() - sf::Vector2f(0, infoText.getLocalBounds().height + 10));
		infoText.setColor(sf::Color::Black);
		infoText.move(1.5, 1.5);
		target.draw(infoText);
		infoText.setColor(sf::Color::White);
		infoText.move(-1.5, -1.5);
		target.draw(infoText);

		background.setSize(sf::Vector2f(30, 30));
		background.move(5, 5);
		background.setFillColor(sf::Color::Transparent);

		for (int x = 0; x < 10; ++x)
		{
			for (int y = 0; y < 5; ++y)
			{
				if (y * 10 + x == mSelectedInventorySlot)
				{
					background.setFillColor(sf::Color(255, 255, 255, 25));
					background.setOutlineColor(sf::Color::White);
				}
				else
				{
					background.setFillColor(sf::Color::Transparent);
					background.setOutlineColor(sf::Color::Black);
				}
					
				target.draw(background);
				auto item = mInventory.getItem(y * 10 + x);
				if (item)
					item->draw(target, background.getPosition() + sf::Vector2f(15, 15));

				background.move(0, 30);
			}

			background.move(30, -30 * 5);
		}
	}
	else if (mCurMenu == Menu_Crafting)
	{
		sf::RectangleShape background(sf::Vector2f(30.f * 10 + 10, 30.f * 5 + 10));
		background.setOrigin(0, 0);
		background.setFillColor(sf::Color(75, 75, 75, 200));
		background.setOutlineThickness(1.f);
		background.setOutlineColor(sf::Color::Black);

		background.setPosition(mPosition + sf::Vector2f(35, -(30.f * 6 + 10)));

		target.draw(background);

		background.setFillColor(sf::Color::Transparent);
		background.setSize(sf::Vector2f(120, 30 * 5));
		background.move(5, 5);

		target.draw(background);
		background.setFillColor(sf::Color(255, 255, 255, 75));

		sf::Text infoText("<Recipe>", mFont, 16U);
		infoText.setPosition(background.getPosition());
		infoText.move(60, 10);

		auto recipes = Recipes::getAllRecipes();

		int i = 0;
		for (std::string& recipe : recipes)
		{
			infoText.setString(recipe);
			{
				auto rect = infoText.getLocalBounds();
				infoText.setOrigin(rect.width / 2.f, rect.height / 2.f);
				background.setSize(sf::Vector2f(120, rect.height + 6.f));
				background.setOrigin(60, rect.height / 2.f);
			}

			background.setPosition(infoText.getPosition());
			
			if (i++ == mSelectedInventorySlot)
				target.draw(background);

			target.draw(infoText);

			if (i++ == mSelectedInventorySlot)
			{
				// Draw crafting recipe
			}

			infoText.move(0, infoText.getLocalBounds().height + 10);
		}
	}
}
