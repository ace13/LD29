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


void StateMachine::update(double dt)
{
	if (mStateStack.empty())
		return;

	mStateStack.back()->update(dt);
}

void StateMachine::handleEvent(const sf::Event& ev)
{
	if (mStateStack.empty())
		return;

	mStateStack.back()->handleEvent(ev);
}

void StateMachine::draw(sf::RenderTarget& target)
{
	for (auto it = mStateStack.begin(); it != mStateStack.end(); ++it)
	{
		(*it)->draw(target);
		if (mDirty)
		{
			mDirty = false;
			return;
		}
	}
}

