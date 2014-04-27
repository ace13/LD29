#pragma once

#include "Actor.hpp"
#include "Inventory.hpp"
#include "../Util/SpriteSheet.hpp"
#include <SFML/Graphics/Font.hpp>

class Player;

class Building : public Actor
{
public:
	Building();
	~Building();

	void doorOpen();
	void update(double dt);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition() const;
	void draw(sf::RenderTarget& target);
	void drawMenu(sf::RenderTarget& target);

private:
	Inventory mInventory;
	double mDoorAnim, mMenuAnim;
	int mSelectedEntry;
	sf::Font mFont;
	SpriteSheet mSheet, mMenuSheet;
	sf::Vector2f mPosition;
	Player* mPlayer;

	friend class Player;
};