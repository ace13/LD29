#include "MenuState.hpp"
#include "Util/FontFinder.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Menu/MainMenu.hpp"
#include "Menu/InGameMenu.hpp"

sf::Font def = FontFinder::findDefaultFont();

MenuState::MenuState(bool ingame) : mDirty(false), mLerp(0)
{
	if (ingame)
		pushPage(new InGameMenuPage(this));
	else
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

	if (mMenuStack.empty())
		return;

	mMenuStack.back()->update(dt);
}

void MenuState::handleEvent(const sf::Event& ev)
{
	if (mMenuStack.empty())
		return;

	mMenuStack.back()->handleEvent(ev);
}

void MenuState::draw(sf::RenderTarget& target)
{
	if (mMenuStack.empty())
	{
		((sf::RenderWindow&)target).close();
		return;
	}

	sf::RectangleShape rect(target.getView().getSize());
	rect.setFillColor(sf::Color(0, 0, 0, 200));
	target.draw(rect);

	sf::Text title("N.A.M.E", def, 64U);

	{
		auto rect = title.getLocalBounds();
		title.setOrigin(rect.width / 2.f, 0);
	}

	title.setPosition(target.getView().getSize().x / 2.f, 0);

	target.draw(title);
	title.move(0, title.getLocalBounds().height + 10);

	title.setCharacterSize(40U);
	title.setString("Not your Average Mining Emulator");
	{
		auto rect = title.getLocalBounds();
		title.setOrigin(rect.width / 2.f, 0);
	}
	target.draw(title);

	title.move(title.getLocalBounds().width / 2.f + 10, 10);
	title.setCharacterSize(16U);
	title.setString("*");
	{
		auto rect = title.getLocalBounds();
		title.setOrigin(rect.width / 2.f, 0);
	}
	target.draw(title);

	title.setPosition(target.getView().getSize().x / 2.f, target.getView().getSize().y);
	title.setString("* Gameplay may actually be average");
	{
		auto rect = title.getLocalBounds();
		title.setOrigin(rect.width / 2.f, rect.height);
	}
	target.draw(title);
	
	std::for_each(mMenuStack.begin(), mMenuStack.end(), [&target](std::shared_ptr<MenuPage>& page) { page->draw(target); });
}
