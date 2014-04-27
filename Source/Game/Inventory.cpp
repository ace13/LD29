#include "Inventory.hpp"
#include "Item.hpp"
#include <algorithm>

Inventory::Inventory(double weightLimit, const sf::Vector2u& slotAmount) : mWeightLimit(weightLimit), mSlots(slotAmount)
{

}

Inventory::~Inventory()
{

}

Item* Inventory::getItem(uint32_t id) const
{
	if (id >= mItems.size())
		return nullptr;

	return mItems.at(id);
}

void Inventory::addItem(Item* item)
{
	mItems.push_back(item);
}

uint32_t Inventory::usedSlots() const
{
	return (mSlots.x * mSlots.y) - mItems.size();
}

double Inventory::getCurWeight() const
{
	double weight = 0;

	std::for_each(mItems.begin(), mItems.end(), [&weight](Item* i) { weight += i->getWeight(); });

	return 0;
}

bool Inventory::maxWeightReached()
{
	if (mWeightLimit < 0)
		return false;

	return getCurWeight() >= mWeightLimit;
}
