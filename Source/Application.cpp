#include "Application.hpp"
#include "Util/FontFinder.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>
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
	sf::Font def = FontFinder::findDefaultFont();
	sf::Text frameInfo("<Frame Info>", def, 18U);
	
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
				}

				mInput.handleEvent(ev);
			}
			
			double dt = std::chrono::duration_cast<std::chrono::duration<double>>(frameTime).count();
			mInput.update(dt);
			mState.update(dt);

			mWindow.clear();

			mState.draw(mWindow);

			std::ostringstream ss;
			ss << "FPS: ~" << mStats.getSmoothFPS() << " (" << mStats.getFPS() << ")\n" << "FrameTime: ~" << mStats.getSmoothFrameTime() << "ms (" << mStats.getFrameTime() << "ms)";
			frameInfo.setString(ss.str());

			mWindow.draw(frameInfo);

			mWindow.display();
		}

		return 0;
	}
	catch (...) ///\TODO Exception handling
	{
	}

	return 1;
}
