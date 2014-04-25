#include "MenuState.hpp"
#include "Util/FontFinder.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

sf::Font def = FontFinder::findDefaultFont();

MenuState::MenuState()
{
}

MenuState::~MenuState()
{

}

void MenuState::update(double dt)
{

}

void MenuState::handleEvent(const sf::Event& ev)
{
}

void MenuState::draw(sf::RenderTarget& target)
{
	sf::Text title("<Title Goes Here>", def, 64U);

	{
		auto rect = title.getLocalBounds();
		title.setOrigin(rect.width / 2.f, 0);
	}

	title.setPosition(target.getView().getSize().x / 2.f, 0);

	target.draw(title);
}
