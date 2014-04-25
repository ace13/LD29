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

	InputSystem& getInputs();

private:
	InputSystem* mInput;

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

	void update(double dt) const;
	void handleEvent(const sf::Event& ev) const;
	void draw(sf::RenderTarget& target) const;

private:
	InputSystem& mInput;
	std::vector<std::shared_ptr<State>> mStateStack;

	// Non-copyable
	StateMachine(const StateMachine&);
	StateMachine(StateMachine&&);
};