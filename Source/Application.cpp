#include "Application.hpp"
#include "Util/FontFinder.hpp"
#include "IntroState.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>
#include <chrono>
#include <thread>

Application::Application() : mState(mInput)
{

}

Application::~Application()
{

}

void Application::init(int argc, char** argv)
{

}

#include "GameState.hpp"

int Application::run()
{
	mWindow.create(sf::VideoMode(1024, 768), "Ludum Dare #29");

	mState.pushState(new GameState());

	sf::Event ev;
	sf::Font def = FontFinder::findDefaultFont();
	sf::Text frameInfo("<Frame Info>", def, 12U);
	sf::View defaultView = mWindow.getDefaultView();
	
	mStats.setSmoothingFactor(FrameStats::Smooth_2x);

	auto framePoint = std::chrono::high_resolution_clock::now();

	try
	{
		while (mWindow.isOpen())
		{
			auto newFrame = std::chrono::high_resolution_clock::now();
			auto frameTime = (newFrame - framePoint);
			framePoint = newFrame;

			mStats.frame(frameTime);

			while (mWindow.pollEvent(ev))
			{
				switch (ev.type)
				{
				case sf::Event::Closed:
					mWindow.close(); break;
				case sf::Event::Resized:
				{
					defaultView.setSize((sf::Vector2f)mWindow.getSize());
					defaultView.setCenter(defaultView.getSize() / 2.f);
				} break;
				}

				//mInput.handleEvent(ev);
				mState.handleEvent(ev);
			}
			
			double dt = std::chrono::duration_cast<std::chrono::duration<double>>(frameTime).count();
			mInput.update(dt);
			mState.update(dt);

			mWindow.clear();
			mWindow.setView(defaultView);
			
			mState.draw(mWindow);

			std::ostringstream ss;
			ss << mStats.getSmoothFPS() << " FPS\n" << mStats.getSmoothFrameTime() << "ms";
			frameInfo.setString(ss.str());

			mWindow.draw(frameInfo);

			mWindow.display();

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		return 0;
	}
	catch (...) ///\TODO Exception handling
	{
	}

	return 1;
}
