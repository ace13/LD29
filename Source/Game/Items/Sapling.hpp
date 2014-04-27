#pragma once

#include "../Item.hpp"
#include <SFML/Graphics/Texture.hpp>

class Sapling : public Item
{
public:
	Sapling();

	void addAmount(float amount) { mAmount += amount; }
	inline float getAmount() const { return mAmount; }
	void removeAmount(float amount) { mAmount -= amount; }

	inline float getWeight() const { return mAmount; }
	inline float maxWeight() const { return 50; }
	inline std::string getName() const { return "Sapling"; }

	void draw(sf::RenderTarget& target, const sf::Vector2f& position);

private:
	sf::Texture mTexture;
	float mAmount;
};