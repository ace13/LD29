#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "InputSystem.hpp"
#include "StateMachine.hpp"
#include "FrameStats.hpp"

class Application
{
public:
	Application();
	~Application();

	void init(int argc, char** argv);

	int run();

private:
	sf::RenderWindow mWindow;
	InputSystem mInput;
	StateMachine mState;
	FrameStats mStats;

	// Non copyable
	Application(const Application&);
	Application(Application&&);
};
