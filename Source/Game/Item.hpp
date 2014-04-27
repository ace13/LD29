#pragma once

class Item
{
public:
	virtual ~Item();

	virtual float getWeight() const = 0;
	virtual bool canStack() const = 0;
	virtual int stackLimit() const = 0;
};