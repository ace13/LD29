#pragma once

#include "StateMachine.hpp"
#include "Game/World.hpp"
#include <SFML/Graphics/View.hpp>

class GameState : public State
{
public:
	GameState();
	~GameState();

	void update(double dt);
	void handleEvent(const sf::Event& ev);
	void draw(sf::RenderTarget& target);

private:
	sf::View mGameView;
	World mWorld;
};