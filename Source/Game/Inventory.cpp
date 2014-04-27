#include "Inventory.hpp"
#include "Item.hpp"
#include <algorithm>

Inventory::Inventory(double weightLimit, uint32_t slotAmount) : mWeightLimit(weightLimit), mSlots(slotAmount)
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

Item* Inventory::getItem(const std::string& key) const
{
	auto it = std::find_if(mItems.begin(), mItems.end(), [key](Item* item) { return item->getName() == key; });
	if (it != mItems.end())
		return *it;

	return nullptr;
}

void Inventory::addItem(Item* item)
{
	auto it = std::find_if(mItems.begin(), mItems.end(), [item](Item* it) { return (item->getName() == it->getName() && (item->getWeight() + it->getWeight()) <= it->maxWeight()); });
	if (it != mItems.end())
	{
		(*it)->addAmount(item->getAmount());
		delete item;
	}
	else
		mItems.push_back(item);
}

void Inventory::removeItem(Item* item)
{
	auto it = std::remove_if(mItems.begin(), mItems.end(), [item](Item* it) { return it == item; });
	if (it != mItems.end())
	{
		mItems.erase(it);
	}
}

double Inventory::getCurWeight() const
{
	double weight = 0;

	std::for_each(mItems.begin(), mItems.end(), [&weight](Item* i) { weight += i->getWeight(); });

	return weight;
}

bool Inventory::maxWeightReached()
{
	if (mWeightLimit < 0)
		return false;

	return getCurWeight() >= mWeightLimit;
}
