#include "StateMachine.hpp"
#include <algorithm>

StateMachine::StateMachine(InputSystem& inp) : mInput(inp), mDirty(false)
{
	mStateStack.reserve(3);
}

StateMachine::~StateMachine()
{

}


void StateMachine::pushState(State* state)
{
	mDirty = true;
	state->mInput = &mInput;
	state->mMachine = this;

	mStateStack.push_back(std::shared_ptr<State>(state));
}

void StateMachine::popState()
{
	mDirty = true;
	mStateStack.pop_back();
}

std::shared_ptr<State> StateMachine::curState() const
{
	return mStateStack.back();
}


void StateMachine::update(double dt) const
{
	for (auto it = mStateStack.rbegin(); it != mStateStack.rend(); ++it)
	{
		(*it)->update(dt);
		if (mDirty)
			return;
	}
}

void StateMachine::handleEvent(const sf::Event& ev) const
{
	for (auto it = mStateStack.rbegin(); it != mStateStack.rend(); ++it)
	{
		(*it)->handleEvent(ev);
		if (mDirty)
			return;
	}
}

void StateMachine::draw(sf::RenderTarget& target) const
{
	for (auto it = mStateStack.rbegin(); it != mStateStack.rend(); ++it)
	{
		(*it)->draw(target);
		if (mDirty)
			return;
	}
}

