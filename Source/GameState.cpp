#include "GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Util/SpriteSheet.hpp"
#include "Util/Resources.hpp"
#include "Util/FontFinder.hpp"

SpriteSheet sheet = Resources::SpriteSheets["tree.png"];
double treeheight = 0;
sf::Font test = FontFinder::findDefaultFont();

GameState::GameState()
{
	mGameView = sf::View(sf::FloatRect(0, 0, 1000, 1000));
}

GameState::~GameState()
{

}

void GameState::update(double dt)
{
	treeheight += dt;
}

void GameState::handleEvent(const sf::Event& ev)
{

}

void GameState::draw(sf::RenderTarget& target)
{
	auto uiView = target.getView();
	target.setView(mGameView);

	///\TODO Game drawing goes here

	auto start = mGameView.getCenter() + sf::Vector2f(0, mGameView.getSize().y / 2.f - 15);

	int floortreeheight = treeheight;
	{
		sf::Sprite treeSprite(sheet.getTexture());
		treeSprite.setOrigin(15, 15);
		treeSprite.setPosition(start);

		if (floortreeheight < 3)
		{
			treeSprite.setTextureRect(sheet.getRect(0, 0));
			target.draw(treeSprite);
		}
		else
		{
			int stems = floortreeheight / 2;
			int crowns = floortreeheight - stems;

			for (int i = 0; i < stems; ++i)
			{
				if (i == 0)
					treeSprite.setTextureRect(sheet.getRect(1, 0));
				else
					treeSprite.setTextureRect(sheet.getRect(2, 0));

				target.draw(treeSprite);
				treeSprite.move(0, -30);
			}
			for (int i = 0; i < crowns; ++i)
			{
				if (i == 0)
					treeSprite.setTextureRect(sheet.getRect(3, 0));
				else if (i == crowns - 1)
					treeSprite.setTextureRect(sheet.getRect(5, 0));
				else
					treeSprite.setTextureRect(sheet.getRect(4, 0));

				target.draw(treeSprite);
				treeSprite.move(0, -30);
			}
		}
	}

	target.setView(uiView);

	///\TODO UI Drawing goes here

	sf::Text text("Tree Growing Simulator 2014", test);
	{
		auto rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2.f, 0);
	}
	text.setPosition(mGameView.getCenter() - sf::Vector2f(0, mGameView.getSize().y / 2.f));
	target.draw(text);
}
