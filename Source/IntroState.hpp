#pragma once

#include "StateMachine.hpp"
#include <SFML/Graphics/Font.hpp>

class IntroState : public State
{
public:
	IntroState();
	~IntroState();

	void update(double dt);
	inline void handleEvent(const sf::Event& ev) { }
	void draw(sf::RenderTarget& target);

private:
	sf::Font mFont;
	double mTime;
};