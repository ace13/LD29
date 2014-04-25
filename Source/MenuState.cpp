#include "MenuState.hpp"
#include "Util/FontFinder.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "Menu/MainMenu.hpp"

sf::Font def = FontFinder::findDefaultFont();

MenuState::MenuState() : mDirty(false), mLerp(0)
{
	pushPage(new MainMenuPage(this));

	mLerp = 1;
}

MenuState::~MenuState()
{

}

void MenuState::pushPage(MenuPage* page)
{
	mMenuStack.push_back(std::shared_ptr<MenuPage>(page));

	mDirty = true;
	mLerp = 0;
}

void MenuState::popPage()
{
	mMenuStack.pop_back();

	mDirty = true;
	mLerp = 0;
}

void MenuState::update(double dt)
{
	if (mDirty)
	{
		if (mMenuStack.empty())
		{
			mDirty = false;
			return;
		}

		mLerp += dt;

		if (mLerp > 1.f)
			mDirty = false;

		float baseHide = 0;
		int i = 0;

		std::for_each(mMenuStack.begin(), mMenuStack.end(), [this, &baseHide, &i](std::shared_ptr<MenuPage>& page) {
			float hide = -baseHide - 15;
			float curHide = page->getHideFactor();
			page->setHideFactor(curHide + (hide - curHide) * mLerp);
			baseHide += page->getWidth();
		});
	}
}

void MenuState::handleEvent(const sf::Event& ev)
{
	if (mMenuStack.empty())
		return;

	mMenuStack.back()->handleEvent(ev);
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

	
	std::for_each(mMenuStack.begin(), mMenuStack.end(), [&target](std::shared_ptr<MenuPage>& page) { page->draw(target); });
}
