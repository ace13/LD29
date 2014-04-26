#include "MainMenu.hpp"
#include "OptionsPage.hpp"
#include "../MenuState.hpp"
#include "../GameState.hpp"

MainMenuPage::MainMenuPage(MenuState* state) : MenuPage(state)
{
	mEntries = { { "New Game", [state](){ auto& m = state->getStateMachine(); m.popState(); m.pushState(new GameState()); } }, { "Options", [state](){ state->pushPage(new OptionsMenuPage(state)); } }, { "Quit Game", [state](){ state->popPage(); } } };
}

MainMenuPage::~MainMenuPage()
{
}