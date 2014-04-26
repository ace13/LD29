#pragma once

#include "Actor.hpp"
#include "../Util/SpriteSheet.hpp"

class Ground : public Actor
{
public:
	enum Type
	{
		Grass,
		Dirt,
		Rock
	};

	Ground();
	~Ground();

	void setType(Type t);

	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const;

	void draw(sf::RenderTarget& target);

private:
	Type mType;
	bool mFlip;
	sf::Vector2f mPosition;
	SpriteSheet mSheet;
};