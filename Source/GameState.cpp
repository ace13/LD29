#include "GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Game/Player.hpp"
#include "InputSystem.hpp"
#include "MenuState.hpp"
#include "Util/FontFinder.hpp"

bool loaded = false;

GameState::GameState()
{
	mGameView = sf::View(sf::FloatRect(0, 0, 666.f, 750));
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

	if (getInputs()["MenuExit"].pressed())
		getStateMachine().pushState(new MenuState(true));
}

void GameState::handleEvent(const sf::Event& ev)
{

}

void GameState::draw(sf::RenderTarget& target)
{
	if (!loaded)
	{
		sf::Font f = FontFinder::findDefaultFont();
		sf::Text loadText("Loading, please wait...", f);
		auto rect = loadText.getLocalBounds();
		loadText.setOrigin(rect.width / 2.f, rect.height / 2.f);
		loadText.setPosition(target.getView().getCenter());

		target.draw(loadText);

		return;
	}
		

	{
		auto size = target.getSize();
		auto gSize = mGameView.getSize();
		mGameView.setSize(gSize.y * ((float)size.x / size.y), gSize.y);
	}

	auto uiView = target.getView();
	target.setView(mGameView);

	target.clear(sf::Color(179, 225, 255));

	///\TODO Game drawing goes here

	mWorld.draw(target);

	target.setView(uiView);

	mWorld.getPlayer()->drawUi(target);
	///\TODO UI Drawing goes here
}
