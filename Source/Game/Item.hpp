#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Item
{
public:
	virtual ~Item() { }

	virtual void addAmount(float amount) = 0;
	virtual void removeAmount(float amount) = 0;
	virtual float getAmount() const = 0;

	virtual float getWeight() const = 0;
	virtual float maxWeight() const = 0;
	virtual std::string getName() const = 0;

	virtual void draw(sf::RenderTarget& target, const sf::Vector2f& position) = 0;
};