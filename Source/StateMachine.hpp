#pragma once

#include <vector>
#include <memory>

namespace sf { class Event; class RenderTarget; }

class InputSystem;
class StateMachine;

class State
{
public:
	State() : mInput(nullptr) { }
	virtual ~State() { }

	virtual void update(double dt) = 0;
	virtual void handleEvent(const sf::Event& ev) = 0;
	virtual void draw(sf::RenderTarget& target) = 0;

	inline InputSystem& getInputs() const { return *mInput; }
	inline StateMachine& getStateMachine() const { return *mMachine; }

private:
	InputSystem* mInput;
	StateMachine* mMachine;

	friend class StateMachine;
};

class StateMachine
{
public:
	StateMachine(InputSystem&);
	~StateMachine();

	void pushState(State*);
	void popState();
	std::shared_ptr<State> curState() const;

	void update(double dt);
	void handleEvent(const sf::Event& ev);
	void draw(sf::RenderTarget& target);

private:
	bool mDirty;
	InputSystem& mInput;
	std::vector<std::shared_ptr<State>> mStateStack;

	// Non-copyable
	StateMachine(const StateMachine&);
	StateMachine(StateMachine&&);
};