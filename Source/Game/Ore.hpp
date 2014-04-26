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

private:
	SpriteSheet mSheet;
	Ground* mGround;
	Type mType;
};