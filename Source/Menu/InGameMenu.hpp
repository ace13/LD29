#pragma once

#include "MenuPage.hpp"

class InGameMenuPage : public MenuPage
{
public:
	InGameMenuPage(MenuState* state);
	~InGameMenuPage();

	inline virtual float getWidth() const { return 230.f; }
	virtual void update(double dt);
};