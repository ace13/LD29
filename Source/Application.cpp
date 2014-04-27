#include "Application.hpp"
#include "Util/FontFinder.hpp"
#include "IntroState.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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

int Application::run()
{
	mWindow.create(sf::VideoMode(1024, 768), "Ludum Dare #29");

	mState.pushState(new IntroState());

	sf::Event ev;
	sf::Font def = FontFinder::findDefaultFont();
	sf::Text frameInfo("<Frame Info>", def, 12U);
	sf::View defaultView = mWindow.getDefaultView();
	
	mStats.setSmoothingFactor(FrameStats::Smooth_2x);

	auto framePoint = std::chrono::high_resolution_clock::now();

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
			
		double dt = std::min(0.11, std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count() / 1000.0);
		mInput.update(dt);
		mState.update(dt);

		mWindow.clear();
		mWindow.setView(defaultView);
			
		mState.draw(mWindow);

		/*
		{
			std::ostringstream ss;
			ss << mStats.getSmoothFPS() << " FPS\n" << mStats.getSmoothFrameTime() << "ms";
			frameInfo.setString(ss.str());

			auto rect = frameInfo.getLocalBounds();
			sf::RectangleShape background(sf::Vector2f(rect.width, rect.height + 8));
			background.setFillColor(sf::Color(0, 0, 0, 128));
			mWindow.draw(background);

			mWindow.draw(frameInfo);
		}
		*/

		mWindow.display();

		std::this_thread::sleep_for(std::chrono::microseconds(500));
	}

	return 0;

	return 1;
}
