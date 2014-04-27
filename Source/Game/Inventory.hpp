#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

class Item;

class Inventory
{
public:
	Inventory(double mWeightLimit, const sf::Vector2u& slots);
	~Inventory();

private:
	double mWeightLimit;
	sf::Vector2u mSlots;
	std::vector<Item*> mItems;
};