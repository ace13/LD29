#pragma once

#include "Actor.hpp"
#include "../Util/SpriteSheet.hpp"

class Building : public Actor
{
public:
	Building();
	~Building();

	void update(double dt);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition() const;
	void draw(sf::RenderTarget& target);

private:
	SpriteSheet mSheet;
	sf::Vector2f mPosition;
};