#include "Application.hpp"

#include <SFML/Window/Event.hpp>
#include <chrono>

Application::Application() : mState(mInput)
{

}

Application::~Application()
{

}

void Application::init(int argc, char** argv)
{

}

int Application::run()
{
	mWindow.create(sf::VideoMode(1024, 768), "Ludum Dare #29");

	sf::Event ev;
	auto framePoint = std::chrono::high_resolution_clock::now();

	try
	{
		while (mWindow.isOpen())
		{
			auto newFrame = std::chrono::high_resolution_clock::now();
			auto frameTime = (newFrame - framePoint);
			framePoint = newFrame;

			while (mWindow.pollEvent(ev))
			{
				switch (ev.type)
				{
				case sf::Event::Closed:
					mWindow.close(); break;
				}

				mInput.handleEvent(ev);
			}

			mState.update(std::chrono::duration_cast<std::chrono::duration<double>>(frameTime).count());

			mWindow.clear();

			mState.draw(mWindow);

			mWindow.display();

			///\TODO Frame statistics
		}

		return 0;
	}
	catch (...) ///\TODO Exception handling
	{
	}

	return 1;
}
