#include "GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Game/Ground.hpp"
#include <vector>

std::vector<Ground*> ground;

GameState::GameState()
{
	mGameView = sf::View(sf::FloatRect(0, 0, 1000, 1000));
}

GameState::~GameState()
{
}

void GameState::update(double dt)
{
	mWorld.update(dt);
}

void GameState::handleEvent(const sf::Event& ev)
{

}

void GameState::draw(sf::RenderTarget& target)
{
	auto uiView = target.getView();
	target.setView(mGameView);

	target.clear(sf::Color(179, 225, 255));

	///\TODO Game drawing goes here

	mWorld.draw(target);

	for (auto g : ground)
	{
		g->draw(target);
	}

	target.setView(uiView);

	///\TODO UI Drawing goes here
}
