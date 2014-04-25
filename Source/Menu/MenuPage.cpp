#include "MenuPage.hpp"
#include "../MenuState.hpp"
#include "../Util/FontFinder.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>

MenuPage::MenuPage(MenuState* state) : mSelectedIndex(-1), mMenuState(state), mHideFactor(0)
{
	
	mMenuFont = FontFinder::findDefaultFont();
}

MenuPage::MenuPage(const MenuPage& rhs) :
	mEntries(rhs.mEntries), mMenuState(rhs.mMenuState), mSelectedEntry(rhs.mSelectedEntry), mSelectedIndex(rhs.mSelectedIndex),
	mMenuFont(rhs.mMenuFont), mHideFactor(rhs.mHideFactor)
{

}

MenuPage::MenuPage(MenuPage&& rhs) :
	mEntries(std::move(rhs.mEntries)), mMenuState(std::move(rhs.mMenuState)), mSelectedEntry(std::move(rhs.mSelectedEntry)),
	mSelectedIndex(std::move(rhs.mSelectedIndex)), mMenuFont(std::move(rhs.mMenuFont)), mHideFactor(std::move(rhs.mHideFactor))
{

}

MenuPage::~MenuPage()
{

}


MenuPage& MenuPage::operator=(MenuPage rhs)
{
	std::swap(mEntries, rhs.mEntries);
	std::swap(mMenuState, rhs.mMenuState);
	std::swap(mSelectedEntry, rhs.mSelectedEntry);
	std::swap(mSelectedIndex, rhs.mSelectedIndex);
	std::swap(mMenuFont, rhs.mMenuFont);
	std::swap(mHideFactor, rhs.mHideFactor);

	return *this;
}

void MenuPage::setHideFactor(float hide)
{
	mHideFactor = hide;
}

float MenuPage::getHideFactor() const
{
	return mHideFactor;
}

void MenuPage::handleEvent(const sf::Event& ev)
{
	auto temp = mSelectedIndex;
	switch (ev.type)
	{
	case sf::Event::KeyReleased:
		if (ev.key.code == sf::Keyboard::Down)
			++mSelectedIndex;
		else if (ev.key.code == sf::Keyboard::Up)
			--mSelectedIndex;
		else if (ev.key.code == sf::Keyboard::Return)
		{
			for (auto& it : mEntries)
			{
				if (it.first == mSelectedEntry)
				{
					it.second();
					return;
				}
			}
		}
	}

	if (temp != mSelectedIndex)
	{
		if (mSelectedIndex > 0 && mSelectedIndex >= mEntries.size())
			mSelectedIndex = 0;
		else if (mSelectedIndex < 0)
			mSelectedIndex = mEntries.size() - 1;

		int i = 0;
		std::find_if(mEntries.begin(), mEntries.end(), [this, &i](const std::pair<std::string, std::function<void()> >& it) {
			if (i++ == mSelectedIndex)
			{
				mSelectedEntry = it.first;
				return true;
			}

			return false;
		});
	}
}

void MenuPage::update(double dt)
{
}

void MenuPage::draw(sf::RenderTarget& target)
{
	sf::Text menuEntry("<Menu Entry Goes Heere>", mMenuFont, 28U);
	auto size = target.getView().getSize();
	
	auto textHeight = menuEntry.getLocalBounds().height;
	auto num = mEntries.size();
	auto test = size.y / 2.f;

	auto totalHeight = (textHeight + ENTRY_PADDING) * num;

	sf::RectangleShape menuLine(sf::Vector2f(5, totalHeight * 1.5f));
	menuLine.setOrigin(2.5f, (totalHeight * 1.5f) / 2.f);
	menuLine.setPosition(-mHideFactor, test);
	menuLine.setFillColor(sf::Color(128, 128, 128));
	target.draw(menuLine);

	menuEntry.setPosition(15 - mHideFactor, test - totalHeight / 2.f);

	if (mSelectedIndex >= 0)
		menuEntry.move(0, ((int)(mEntries.size() / 2) - mSelectedIndex) * textHeight);

	std::string selectedEntry = mSelectedEntry;

	float pad = ENTRY_PADDING;
	std::for_each(mEntries.begin(), mEntries.end(), [&target, &menuEntry, &selectedEntry, textHeight, pad](const std::pair<std::string, std::function<void()> >& it) {
		menuEntry.setColor(sf::Color::White);
		menuEntry.setString(it.first);
		
		if (selectedEntry == it.first)
			menuEntry.setColor(sf::Color::Yellow);

		target.draw(menuEntry);

		menuEntry.move(0, textHeight + pad);
	});
}
