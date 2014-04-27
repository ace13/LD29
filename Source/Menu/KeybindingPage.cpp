#include "KeybindingPage.hpp"
#include "../MenuState.hpp"
#include "../InputSystem.hpp"
#include "../Util/ShapeDraw.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

KeybindingPage::KeybindingPage(MenuState* state) : MenuPage(state), mAnim(0)
{
	auto binds = state->getInputs().getValidBinds();
	
	mEntries.reserve(binds.size() + 1);

	for (auto& bind : binds)
	{
		mEntries.push_back({ bind, []() {} });
	}

	mEntries.push_back({ "Back", [state]() { state->popPage(); } });

}
KeybindingPage::~KeybindingPage()
{

}

void KeybindingPage::handleEvent(const sf::Event& ev)
{
	if (mRebinding.empty())
		return;

	bool bound = false;

	switch (ev.type)
	{
	case sf::Event::KeyPressed:
		mMenuState->getInputs().rebind(mRebinding, InputSystem::Bind::Type_Keyboard, InputSystem::Bind::BindData::KB{ ev.key.code }); bound = true; break;
	case sf::Event::JoystickButtonPressed:
		mMenuState->getInputs().rebind(mRebinding, InputSystem::Bind::Type_JoyButton, InputSystem::Bind::BindData::JB{ ev.joystickButton.joystickId, ev.joystickButton.button }); bound = true; break;
	case sf::Event::JoystickMoved:
	{
		auto m = ev.joystickMove;
		if (std::abs(m.position) > 75)
		{
			mMenuState->getInputs().rebind(mRebinding, InputSystem::Bind::Type_JoyAxis, InputSystem::Bind::BindData::JA{ m.joystickId, m.axis, m.position < 0 });
			bound = true;
		}
	} break;
	}

	if (bound)
		mRebinding.clear();
}

void KeybindingPage::update(double dt)
{
	mAnim += dt;

	if (!mRebinding.empty())
		return;

	auto& inp = mMenuState->getInputs();

	if (inp["MenuEnter"].pressed())
	{
		mRebinding = mSelectedEntry;
	}

	MenuPage::update(dt);
}

void KeybindingPage::draw(sf::RenderTarget& target)
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

	menuEntry.setPosition(15 - mHideFactor, test - totalHeight / 2.f);

	if (mSelectedIndex >= 0)
		menuEntry.move(0, ((int)(mEntries.size() / 2) - mSelectedIndex) * textHeight);

	std::string selectedEntry = mSelectedEntry;
	Shapes::BoundInput inpShape;

	int i = 0;
	std::for_each(mEntries.begin(), mEntries.end(), [&target, &menuEntry, this, textHeight, &i, &inpShape](const std::pair<std::string, std::function<void()> >& it) {
		menuEntry.setColor(sf::Color::White);
		menuEntry.setString(it.first);

		if (mSelectedEntry == it.first)
			menuEntry.setColor(sf::Color::Yellow);
		if (mRebinding == it.first)
			menuEntry.setColor(sf::Color(255, 127, 0));

		target.draw(menuEntry);

		if (i < mEntries.size() - 1)
		{
			auto pos = menuEntry.getPosition();
			auto inp = mMenuState->getInputs()[it.first];
			inpShape.setBind(inp);

			if ((int)mAnim % 2 == 0)
				inpShape.setPressed(true);

			inpShape.setPosition(pos.x + 125, pos.y);

			target.draw(inpShape);
		}

		menuEntry.move(0, textHeight + ENTRY_PADDING);
		++i;
	});
}