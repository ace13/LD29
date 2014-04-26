#include "GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Game/Actor.hpp"

bool loaded = false;

GameState::GameState()
{
	mGameView = sf::View(sf::FloatRect(0, 0, 1000, 1000));
	mGameView.setCenter(0, -15 * WORLD_HEIGHT);
}

GameState::~GameState()
{
}

void GameState::update(double dt)
{
	if (!loaded)
	{
		mWorld.generateWorld(getInputs());
		loaded = true;
	}

	mWorld.update(dt);

	mGameView.setCenter(((Actor*)mWorld.getPlayer())->getPosition());
}

void GameState::handleEvent(const sf::Event& ev)
{

}

void GameState::draw(sf::RenderTarget& target)
{
	{
		auto size = target.getSize();
		mGameView.setSize(1000.f, 1000.f * ((float)size.y / size.x));
	}

	auto uiView = target.getView();
	target.setView(mGameView);

	target.clear(sf::Color(179, 225, 255));

	///\TODO Game drawing goes here

	mWorld.draw(target);

	target.setView(uiView);

	///\TODO UI Drawing goes here
}
