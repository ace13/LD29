#include "GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Util/FontFinder.hpp"
#include "Game/Tree.hpp"

sf::Font test = FontFinder::findDefaultFont();
Tree* t;

GameState::GameState()
{
	mGameView = sf::View(sf::FloatRect(0, 0, 1000, 1000));
	t = new Tree();
}

GameState::~GameState()
{
	delete t;
}

void GameState::update(double dt)
{
}

void GameState::handleEvent(const sf::Event& ev)
{

}

void GameState::draw(sf::RenderTarget& target)
{
	auto uiView = target.getView();
	target.setView(mGameView);

	///\TODO Game drawing goes here

	t->draw(target);


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
