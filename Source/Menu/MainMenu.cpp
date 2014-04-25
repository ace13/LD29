#include "MainMenu.hpp"
#include "OptionsPage.hpp"
#include "../MenuState.hpp"

MainMenuPage::MainMenuPage(MenuState* state) : MenuPage(state)
{
	mEntries = { { "New Game", [state](){ } }, { "Options", [state](){ state->pushPage(new OptionsMenuPage(state)); } }, { "Quit Game", [state](){ state->popPage(); } } };
}

MainMenuPage::~MainMenuPage()
{
}