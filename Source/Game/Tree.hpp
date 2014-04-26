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

	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const;

	void chop();
	void draw(sf::RenderTarget& target);
	void update(double dt);

private:
	sf::Vector2f mPos;
	uint8_t mStemLength, mCrownLength;
	SpriteSheet mSheet;
};