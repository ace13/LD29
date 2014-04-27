#pragma once

#include "StateMachine.hpp"
#include "Util/OBJLoader.hpp"
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
	OBJFile mCard;
	sf::Font mFont;
	double mTime;
};