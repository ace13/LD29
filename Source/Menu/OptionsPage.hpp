#pragma once

#include "MenuPage.hpp"

class OptionsMenuPage : public MenuPage
{
public:
	OptionsMenuPage(MenuState* state);
	~OptionsMenuPage();

	virtual void handleEvent(const sf::Event& ev);
	virtual void update(double dt);
	virtual void draw(sf::RenderTarget& target);

private:
	uint8_t mSoundVol, mMusicVol;
};