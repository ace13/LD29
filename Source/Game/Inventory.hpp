#pragma once

#include <SFML/System/Vector2.hpp>

class Item;

class Inventory
{
public:
	Inventory(double mWeightLimit, const sf::Vector2u& slots);
	~Inventory();

private:
	double mWeightLimit;
	sf::Vector2u mSlots;

};