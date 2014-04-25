#pragma once

#include "MenuPage.hpp"

class KeybindingPage : public MenuPage
{
public:
	KeybindingPage(MenuState* state);
	~KeybindingPage();

	virtual void handleEvent(const sf::Event& ev);
	virtual void update(double dt);
	virtual void draw(sf::RenderTarget& target);

private:
	uint8_t mSoundVol, mMusicVol;
	std::string mRebinding;
};