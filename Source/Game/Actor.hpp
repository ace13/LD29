#pragma once

#include <SFML/System/Vector2.hpp>

class Actor
{
public:
	virtual ~Actor() { }

	virtual sf::Vector2f getPosition() const = 0;
};