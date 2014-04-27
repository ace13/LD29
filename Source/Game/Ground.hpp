#pragma once

#include "Actor.hpp"
#include "../Util/SpriteSheet.hpp"

class Ore;

class Ground : public Actor
{
public:
	enum Type
	{
		Grass,
		Dirt,
		Rock
	};

	Ground(Type t);
	~Ground();

	void dig(double dt);
	bool dug() const;

	bool hasOre() const { return mOre; }

	void genOre();
	void setType(Type t);

	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const;

	void update(double dt);
	void draw(sf::RenderTarget& target);

private:
	Ore* mOre;
	Type mType;
	bool mFlip;
	double mDug;
	sf::Vector2f mPosition;
	SpriteSheet mSheet, mBreakSheet;
};