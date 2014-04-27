#pragma once

#include <string>

class Item
{
public:
	virtual ~Item() { }

	virtual void addAmount(float amount) = 0;
	virtual float getAmount() const = 0;

	virtual float getWeight() const = 0;
	virtual float maxWeight() const = 0;
	virtual std::string getName() const = 0;
};