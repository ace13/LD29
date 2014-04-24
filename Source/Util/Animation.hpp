#pragma once

#include "SpriteSheet.hpp"
#include <list>

class Animation
{
public:
	struct Frame
	{
		sf::IntRect TextureRect;
		double      Time;
	};

	Animation();
	Animation(std::initializer_list<Frame> list);
	Animation(const Animation&);
	Animation(Animation&&);
	~Animation();

	Animation& operator=(Animation);

	void addFrame(const Frame& frame);

	void update(double dt);
	sf::IntRect getRect() const;

private:
	double mTime;
	std::list<Frame> mFrames;
};