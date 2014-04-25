#include "KeybindingPage.hpp"
#include "../MenuState.hpp"
#include "../InputSystem.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

namespace
{
	std::string getKeyName(sf::Keyboard::Key k)
	{
#define K(key) case sf::Keyboard::key: return #key
		switch (k)
		{
			K(A); K(B); K(C); K(D); K(E); K(F); K(G); K(H); K(I); K(J); K(K); K(L); K(M); K(N);
			K(O); K(P); K(Q); K(R); K(S); K(T); K(U); K(V); K(W); K(X); K(Y); K(Z); K(Num0);
			K(Num1); K(Num2); K(Num3); K(Num4); K(Num5); K(Num6); K(Num7); K(Num8); K(Num9);
			K(Escape); K(LControl); K(LShift); K(LAlt); K(LSystem); K(RControl); K(RShift);
			K(RAlt); K(RSystem); K(Menu); K(LBracket); K(RBracket); K(SemiColon); K(Comma);
			K(Period); K(Quote); K(Slash); K(BackSlash); K(Tilde); K(Equal); K(Dash); K(Space);
			K(Return); K(BackSpace); K(Tab); K(PageUp); K(PageDown); K(End); K(Home); K(Insert);
			K(Delete); K(Add); K(Subtract); K(Multiply); K(Divide); K(Left); K(Right); K(Up);
			K(Down); K(Numpad0); K(Numpad1); K(Numpad2); K(Numpad3); K(Numpad4); K(Numpad5);
			K(Numpad6); K(Numpad7); K(Numpad8); K(Numpad9); K(F1); K(F2); K(F3); K(F4); K(F5);
			K(F6); K(F7); K(F8); K(F9); K(F10); K(F11); K(F12); K(F13); K(F14); K(F15); K(Pause);
		default:
			return "";
		}
#undef K
	}

	std::string getAxisName(sf::Joystick::Axis a)
	{
		switch (a)
		{
		case sf::Joystick::Axis::X:
			return "LS X";
		case sf::Joystick::Axis::Y:
			return "LS Y";
		case sf::Joystick::Axis::R:
			return "RS X";
		case sf::Joystick::Axis::U:
			return "RS Y";
		case sf::Joystick::Axis::Z:
			return "LT";
		case sf::Joystick::Axis::V:
			return "RT";
		case sf::Joystick::Axis::PovX:
			return "POV X";
		case sf::Joystick::Axis::PovY:
			return "POV Y";
		}
	}

	std::string getButtonName(uint32_t b)
	{
		switch (b)
		{
		case 0: return "A";
		case 1: return "B";
		case 2: return "X";
		case 3: return "Y";
		case 4: return "LB";
		case 5: return "RB";
		case 6: return "Back";
		case 7: return "Start";
		case 8: return "LS";
		case 9: return "RS";
		}
	}
}

KeybindingPage::KeybindingPage(MenuState* state) : MenuPage(state)
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
	MenuPage::handleEvent(ev);
}

void KeybindingPage::update(double dt)
{

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

	int i = 0;
	std::for_each(mEntries.begin(), mEntries.end(), [&target, &menuEntry, this, textHeight, &i](const std::pair<std::string, std::function<void()> >& it) {
		menuEntry.setColor(sf::Color::White);
		menuEntry.setString(it.first);

		if (mSelectedEntry == it.first)
			menuEntry.setColor(sf::Color::Yellow);

		target.draw(menuEntry);

		if (i < mEntries.size() - 1)
		{
			auto pos = menuEntry.getPosition();
			menuEntry.setPosition(pos.x + 125, pos.y);

			auto inp = mMenuState->getInputs()[it.first];
			switch (inp.getType())
			{
			case InputSystem::Bind::Type_Keyboard:
				menuEntry.setString(getKeyName((sf::Keyboard::Key)inp.getData().Keyboard.Key));
				break;
			case InputSystem::Bind::Type_JoyAxis:
				menuEntry.setString(getAxisName((sf::Joystick::Axis)inp.getData().JoyAxis.Axis) + (inp.getData().JoyAxis.Negative ? "-" : "+"));
				break;
			case InputSystem::Bind::Type_JoyButton:
				menuEntry.setString(getButtonName(inp.getData().JoyButton.Button));
				break;
			}
			target.draw(menuEntry);

			menuEntry.setPosition(pos);
		}

		menuEntry.move(0, textHeight + ENTRY_PADDING);
		++i;
	});
}