#include "InGameMenu.hpp"
#include "OptionsPage.hpp"
#include "../MenuState.hpp"
#include "../InputSystem.hpp"

InGameMenuPage::InGameMenuPage(MenuState* state) : MenuPage(state)
{
	mEntries = { { "Continue Game", [state](){ auto& m = state->getStateMachine(); m.popState(); } }, { "Options", [state](){ state->pushPage(new OptionsMenuPage(state)); } }, { "Quit Game", [state](){ state->popPage(); } } };
}

InGameMenuPage::~InGameMenuPage()
{
}

void InGameMenuPage::update(double dt)
{
	if (mMenuState->getInputs()["MenuExit"].pressed())
	{
		mMenuState->getStateMachine().popState();
		return;
	}

	MenuPage::update(dt);
}