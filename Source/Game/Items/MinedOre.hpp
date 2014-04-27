#pragma once

#include "../Item.hpp"
#include "../Ore.hpp"
#include "../../Util/SpriteSheet.hpp"

class MinedOre : public Item
{
public:
	MinedOre(Ore::Type t);

	void addAmount(float amount) { mAmount += amount; }
	inline float getAmount() const { return mAmount; }

	inline float getWeight() const { return mAmount * 5; }
	inline float maxWeight() const { return 50; }
	inline std::string getName() const;

	void draw(sf::RenderTarget& target, const sf::Vector2f& position);

private:
	SpriteSheet mSheet;
	Ore::Type mType;
	float mAmount;
};