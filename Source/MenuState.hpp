#pragma once

#include "StateMachine.hpp"
#include "Menu/MenuPage.hpp"
#include <vector>

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void update(double dt);
	void handleEvent(const sf::Event& ev);
	void draw(sf::RenderTarget& target);

	void pushPage(const MenuPage&);
	void popPage();

private:
	std::vector<MenuPage> mMenuStack;
	float mLerp;
	bool mDirty;
};
