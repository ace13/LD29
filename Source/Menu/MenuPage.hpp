#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>

#include <cstdint>
#include <functional>
#include <vector>
#include <map>

namespace sf { class Event; }
class MenuState;

class MenuPage
{
public:
	MenuPage(MenuState* state);
	MenuPage(const MenuPage&);
	MenuPage(MenuPage&&);
	~MenuPage();

	MenuPage& operator=(MenuPage);

	void setHideFactor(float);
	float getHideFactor() const;

	void handleEvent(const sf::Event& ev);
	void update(double dt);
	void draw(sf::RenderTarget& target);

private:
	std::vector<std::pair<std::string, std::function<void()> > > mEntries;
	MenuState* mMenuState;
	std::string mSelectedEntry;
	int16_t mSelectedIndex;
	sf::Font mMenuFont;
	float mHideFactor;
};
