#pragma once

#include "Actor.hpp"

class Building : public Actor
{
public:
	Building();
	~Building();

	virtual void update(double dt);
	virtual sf::Vector2f getPosition() const;
	virtual void draw(sf::RenderTarget& target);

private:

};