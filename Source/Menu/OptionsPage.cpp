#include "OptionsPage.hpp"
#include "../MenuState.hpp"

OptionsMenuPage::OptionsMenuPage(MenuState* state) : MenuPage(state)
{
	mEntries = { { "Sound: 100%", [](){} }, { "Music: 100%", [](){} }, { "Keybinds", [](){} }, { "Back", [state](){ state->popPage(); } } };
}

OptionsMenuPage::~OptionsMenuPage()
{
}