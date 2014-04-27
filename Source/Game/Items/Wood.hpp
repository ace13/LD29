#pragma once

#include "../Item.hpp"

class Wood : public Item
{
public:
	Wood();

	void addAmount(float amount);
	inline float getAmount() const { return mAmount; }

	inline float getWeight() const { return mAmount; }
	inline float maxWeight() const { return 50; }
	inline std::string getName() const { return "Wood"; }

private:
	float mAmount;
};