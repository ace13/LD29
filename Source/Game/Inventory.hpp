#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cstdint>

class Item;

class Inventory
{
public:
	Inventory(double mWeightLimit, const sf::Vector2u& slotAmount);
	~Inventory();

	Item* getItem(uint32_t id = 0) const;
	Item* getItem(const std::string& name) const;
	void addItem(Item* item);
	void removeItem(Item* item);
	inline bool empty() const { return mItems.empty(); }
	
	uint32_t usedSlots() const;
	inline uint32_t freeSlots() const { return (mSlots.x + mSlots.y) - usedSlots(); }

	double getCurWeight() const;
	inline double getWeightLimit() const { return mWeightLimit; }
	bool maxWeightReached();

private:
	double mWeightLimit;
	sf::Vector2u mSlots;
	std::vector<Item*> mItems;
};