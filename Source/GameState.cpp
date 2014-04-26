#include "GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

GameState::GameState()
{
	mGameView = sf::View(sf::FloatRect(0, 0, 1000, 1000));
}

GameState::~GameState()
{

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

	target.setView(uiView);

	///\TODO UI Drawing goes here
}
