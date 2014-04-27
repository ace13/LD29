#pragma once

#include "Ground.hpp"
#include "../Util/SpriteSheet.hpp"

class Ore
{
public:
	enum Type
	{
		Gold,
		Iron,
		Diamond,
		Emerald,
		Sulfur,
		Max
	};

	static uint32_t OreWeight(Type t, float depth);

	Ore(Ground* g, Type t);
	~Ore();

	void update(double dt);
	void draw(sf::RenderTarget& target);

	float getAmount() const { return mAmount; }
	void removeAmount(float amount) { mAmount -= amount; }

	Type getType() const { return mType; }

private:
	float mAmount;

	SpriteSheet mSheet;
	Ground* mGround;
	Type mType;
};