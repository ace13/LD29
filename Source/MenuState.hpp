#pragma once

#include "StateMachine.hpp"
#include <SFML/Graphics/View.hpp>

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void update(double dt);
	void handleEvent(const sf::Event& ev);
	void draw(sf::RenderTarget& target);

private:
	sf::View mMenuView;
};
