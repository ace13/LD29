#pragma once

#include "Actor.hpp"
#include "Inventory.hpp"
#include "../Util/SpriteSheet.hpp"
#include <SFML/Graphics/Font.hpp>

class Player;

class Building : public Actor
{
public:
	enum MenuType
	{
		Menu_Main,
		Menu_Inventory,
		Menu_Crafting
	};

	Building();
	~Building();

	void doorOpen();
	void update(double dt);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition() const;
	void draw(sf::RenderTarget& target);

	void drawMenu(sf::RenderTarget& target, MenuType type = Menu_Main);

private:
	MenuType mCurMenu;
	Inventory mInventory;
	double mDoorAnim, mMenuAnim;
	int mSelectedEntry, mSelectedInventorySlot;
	sf::Font mFont;
	SpriteSheet mSheet, mMenuSheet;
	sf::Vector2f mPosition;
	Player* mPlayer;

	friend class Player;
};