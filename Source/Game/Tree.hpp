#pragma once

#include "Actor.hpp"
#include "../Util/SpriteSheet.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdint>

class Tree : public Actor
{
public:
	Tree();
	~Tree();

	sf::Vector2f getPosition() const;
	void draw(sf::RenderTarget& target);

private:
	uint8_t mStemLength, mCrownLength;
	SpriteSheet mSheet;
};