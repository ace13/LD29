#include "MenuState.hpp"
#include "Util/FontFinder.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

sf::Font def = FontFinder::findDefaultFont();

MenuState::MenuState() : mDirty(false), mLerp(0)
{
	pushPage(MenuPage(this));

	mLerp = 1;
}

MenuState::~MenuState()
{

}

void MenuState::pushPage(const MenuPage& page)
{
	mMenuStack.push_back(page);

	mDirty = true;
	mLerp = 0;
}

void MenuState::popPage()
{
	mMenuStack.pop_back();

	if (mMenuStack.empty())
		exit(0);

	mDirty = true;
	mLerp = 0;
}

void MenuState::update(double dt)
{
	if (mDirty)
	{
		mLerp += dt;

		if (mLerp > 1.f)
			mDirty = false;

		float baseHide = 180;
		int i = 0;

		std::for_each(mMenuStack.begin(), mMenuStack.end(), [this, baseHide, &i](MenuPage& page) {
			float hide = -baseHide * i++;
			float curHide = page.getHideFactor();
			page.setHideFactor(curHide + (hide - curHide) * mLerp);
		});
	}
}

void MenuState::handleEvent(const sf::Event& ev)
{
	mMenuStack.back().handleEvent(ev);
}

void MenuState::draw(sf::RenderTarget& target)
{
	sf::Text title("<Title Goes Here>", def, 64U);

	{
		auto rect = title.getLocalBounds();
		title.setOrigin(rect.width / 2.f, 0);
	}

	title.setPosition(target.getView().getSize().x / 2.f, 0);

	target.draw(title);

	
	std::for_each(mMenuStack.begin(), mMenuStack.end(), [&target](MenuPage& page) { page.draw(target); });
}
