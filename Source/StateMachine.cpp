#include "StateMachine.hpp"
#include <algorithm>

StateMachine::StateMachine(InputSystem& inp) : mInput(inp)
{
	mStateStack.reserve(3);
}

StateMachine::~StateMachine()
{

}


void StateMachine::pushState(State* state)
{
	mStateStack.push_back(std::shared_ptr<State>(state));
}

void StateMachine::popState()
{
	mStateStack.pop_back();
}

std::shared_ptr<State> StateMachine::curState() const
{
	return mStateStack.back();
}


void StateMachine::update(double dt) const
{
	std::for_each(mStateStack.rbegin(), mStateStack.rend(), [dt](const std::shared_ptr<State>& state) { state->update(dt); });
}

void StateMachine::handleEvent(const sf::Event& ev) const
{
	std::for_each(mStateStack.rbegin(), mStateStack.rend(), [&ev](const std::shared_ptr<State>& state) { state->handleEvent(ev); });
}

void StateMachine::draw(sf::RenderTarget& target) const
{
	std::for_each(mStateStack.rbegin(), mStateStack.rend(), [&target](const std::shared_ptr<State>& state) { state->draw(target); });
}

