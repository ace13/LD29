#include "IntroState.hpp"
#include "MenuState.hpp"
#include "Util/FontFinder.hpp"
#include "InputSystem.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

IntroState::IntroState() : mTime(0)
{
	mFont = FontFinder::findDefaultFont();
	mCard.loadFromFile("ace.obj");
}

IntroState::~IntroState()
{

}

void IntroState::update(double dt)
{
	mTime += dt / 6;

	mCard.update(dt);

	if (getInputs()["MenuExit"].pressed())
		mTime = 1;

	if (mTime > 1)
	{
		auto stateKeeper = getStateMachine().curState();
		getStateMachine().popState();
		getStateMachine().pushState(new MenuState());
	}
}

void IntroState::draw(sf::RenderTarget& target)
{
	target.draw(mCard);

	sf::Text introText("<Intro placeholder>", mFont);
	{
		auto rect = introText.getLocalBounds();
		introText.setOrigin(rect.width / 2.f, rect.height / 2.f);
	}

	introText.setPosition(target.getView().getCenter());

	//target.draw(introText);

	introText.setString("Made by Alexander \"Ace\" / \"Ananace\" Olofsson for Ludum Dare #29 (Beneath the Surface)");
	introText.setCharacterSize(15U);
	introText.setColor(sf::Color(128, 128, 128));

	{
		auto rect = introText.getLocalBounds();
		introText.setOrigin(rect.width / 2.f, rect.height / 2.f);
		introText.setPosition(target.getView().getCenter().x, target.getView().getSize().y - rect.height);
	}

	target.draw(introText);
}