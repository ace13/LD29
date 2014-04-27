#pragma once

#include "Actor.hpp"
#include "Inventory.hpp"
#include "../Util/SpriteSheet.hpp"

class InputSystem;
class Building;

class Player : public Actor
{
public:
	Player(InputSystem& sys);
	~Player();

	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const;

	void update(double dt);
	void draw(sf::RenderTarget& target);
	void drawUi(sf::RenderTarget& target);

private:
	SpriteSheet mSheet;
	Inventory mInventory;
	InputSystem& mInp;
	sf::Vector2f mPosition, mSpeed;
	float mFallSpeed, mAnim;
	bool mOnGround;

	Building* mInBuilding;
};
