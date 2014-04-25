#pragma once

#include "MenuPage.hpp"
#include "../Util/ShapeDraw.hpp"

class OptionsMenuPage : public MenuPage
{
public:
	OptionsMenuPage(MenuState* state);
	~OptionsMenuPage();

	inline virtual float getWidth() const { return 240.f; }

	virtual void handleEvent(const sf::Event& ev);
	virtual void update(double dt);
	virtual void draw(sf::RenderTarget& target);

private:
	double mSoundVol, mMusicVol;
	Shapes::RadialProgressBar mSoundBar, mMusicBar;
};