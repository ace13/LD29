#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cstdint>
#include <string>

class Item;

class Inventory
{
public:
	Inventory(double mWeightLimit, uint32_t slotAmount);
	~Inventory();

	Item* getItem(uint32_t id = 0) const;
	Item* getItem(const std::string& name) const;
	void addItem(Item* item);
	void removeItem(Item* item);
	inline bool empty() const { return mItems.empty(); }
	
	inline uint32_t usedSlots() const { return mItems.size(); }
	inline uint32_t freeSlots() const { return (mSlots) - usedSlots(); }
	inline uint32_t totalSlots() const { return mSlots; }

	double getCurWeight() const;
	inline double getWeightLimit() const { return mWeightLimit; }
	bool maxWeightReached();

private:
	double mWeightLimit;
	uint32_t mSlots;
	std::vector<Item*> mItems;
};