#include "OptionsPage.hpp"
#include "KeybindingPage.hpp"
#include "../InputSystem.hpp"
#include "../MenuState.hpp"
#include "../Util/ShapeDraw.hpp"

#include "../Settings.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

OptionsMenuPage::OptionsMenuPage(MenuState* state) : MenuPage(state), mSoundVol(50), mMusicVol(50)
{
	mEntries = { { "Sound: %d%%", [](){} }, { "Music: %d%%", [](){} }, { "Keybinds", [state](){ state->pushPage(new KeybindingPage(state)); } }, { "Back", [state](){ state->popPage(); } } };

	mSoundVol = Settings::getSetting<int>("volume", "iSound");
	mMusicVol = Settings::getSetting<int>("volume", "iMusic");

	mSoundBar.setMaxValue(100);
	mSoundBar.setRadius(10);
	mSoundBar.setOutlineColor(sf::Color(75, 75, 75));
	mSoundBar.setBackgroundColor(sf::Color(0, 28, 0));
	mSoundBar.setForegroundColor(sf::Color(0, 150, 0));
	mSoundBar.setValue(mSoundVol);
	mSoundBar.setOrigin(10, 10);
	mSoundBar.setCaching(true);

	mMusicBar.setMaxValue(100);
	mMusicBar.setRadius(10);
	mMusicBar.setOutlineColor(sf::Color(75, 75, 75));
	mMusicBar.setBackgroundColor(sf::Color(0, 28, 0));
	mMusicBar.setForegroundColor(sf::Color(0, 150, 0));
	mMusicBar.setValue(mMusicVol);
	mMusicBar.setOrigin(10, 10);
	mMusicBar.setCaching(true);
}

OptionsMenuPage::~OptionsMenuPage()
{
	Settings::setSetting<int>("volume", "iSound", mSoundVol);
	Settings::setSetting<int>("volume", "iMusic", mMusicVol);
}

void OptionsMenuPage::handleEvent(const sf::Event& ev)
{
	MenuPage::handleEvent(ev);
}

void OptionsMenuPage::update(double dt)
{
	auto& inp = mMenuState->getInputs();

	if (inp["MenuLeft"].curValue() > 0.5f)
	{
		if (mSelectedIndex == 0)
			mSoundVol = std::max(0.0, (mSoundVol - inp["MenuLeft"].curValue() * dt * 25));
		else if (mSelectedIndex == 1)
			mMusicVol = std::max(0.0, (mMusicVol - inp["MenuLeft"].curValue() * dt * 25));
	}
	else if (inp["MenuRight"].curValue() > 0.5f)
	{
		if (mSelectedIndex == 0)
			mSoundVol = std::min(100.0, (mSoundVol + inp["MenuRight"].curValue() * dt * 25));
		else if (mSelectedIndex == 1)
			mMusicVol = std::min(100.0, (mMusicVol + inp["MenuRight"].curValue() * dt * 25));
	}

	mSoundBar.setValue((int)mSoundVol);
	mMusicBar.setValue((int)mMusicVol);

	MenuPage::update(dt);
}

void OptionsMenuPage::draw(sf::RenderTarget& target)
{
	sf::Text menuEntry("<Menu Entry Goes Heere>", mMenuFont, 28U);
	auto size = target.getView().getSize();

	auto textHeight = menuEntry.getLocalBounds().height;
	auto num = mEntries.size();
	auto test = size.y * MENU_POSITION_FACTOR;

	auto totalHeight = (textHeight + ENTRY_PADDING) * num;

	sf::RectangleShape menuLine(sf::Vector2f(5, totalHeight * 1.5f));
	menuLine.setOrigin(2.5f, (totalHeight * 1.5f) / 2.f);
	menuLine.setPosition(-mHideFactor, test);
	menuLine.setFillColor(sf::Color(128, 128, 128));
	target.draw(menuLine);

	
	
	mSoundBar.setPosition(15 - mHideFactor + 180, test - totalHeight / 2.f + 5);
	menuEntry.setPosition(15 - mHideFactor, test - totalHeight / 2.f);

	if (mSelectedIndex >= 0)
	{
		mSoundBar.move(0, ((int)(mEntries.size() / 2) - mSelectedIndex) * textHeight);
		menuEntry.move(0, ((int)(mEntries.size() / 2) - mSelectedIndex) * textHeight);
	}

	target.draw(mSoundBar);

	mMusicBar.setPosition(mSoundBar.getPosition());
	mMusicBar.move(-5, textHeight + ENTRY_PADDING);

	target.draw(mMusicBar);

	int i = 0;
	std::for_each(mEntries.begin(), mEntries.end(), [&target, &menuEntry, this, textHeight, &i](const std::pair<std::string, std::function<void()> >& it) {
		menuEntry.setColor(sf::Color::White);

		if (i == 0)
		{
			char tmp[12];
			sprintf(tmp, it.first.c_str(), (int)mSoundVol);
			menuEntry.setString(tmp);
		}
		else if (i == 1)
		{
			char tmp[12];
			sprintf(tmp, it.first.c_str(), (int)mMusicVol);
			menuEntry.setString(tmp);
		}
		else
			menuEntry.setString(it.first);

		if (mSelectedEntry == it.first)
			menuEntry.setColor(sf::Color::Yellow);

		target.draw(menuEntry);

		menuEntry.move(0, textHeight + ENTRY_PADDING);
		++i;
	});
}