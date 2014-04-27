#pragma once

#include "../Item.hpp"
#include "../../Util/SpriteSheet.hpp"

class Wood : public Item
{
public:
	Wood();

	void addAmount(float amount);
	inline float getAmount() const { return mAmount; }
	void removeAmount(float amount) { mAmount -= amount; }

	inline float getWeight() const { return mAmount; }
	inline float maxWeight() const { return 50; }
	inline std::string getName() const { return "Wood"; }

	void draw(sf::RenderTarget& target, const sf::Vector2f& position);

private:
	SpriteSheet mSheet;
	float mAmount;
};