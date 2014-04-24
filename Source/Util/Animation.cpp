#include "Animation.hpp"

Animation::Animation() : mTime(0)
{

}

Animation::Animation(std::initializer_list<Frame> list) : mTime(0)
{
	std::for_each(list.begin(), list.end(), [this](const Frame& f) { addFrame(f); });
}

Animation::Animation(const Animation& rhs) : mTime(rhs.mTime), mFrames(rhs.mFrames)
{

}

Animation::Animation(Animation&& rhs) : mTime(std::move(rhs.mTime)), mFrames(std::move(rhs.mFrames))
{

}

Animation::~Animation()
{

}


Animation& Animation::operator=(Animation rhs)
{
	std::swap(mTime, rhs.mTime);
	std::swap(mFrames, rhs.mFrames);

	return *this;
}


void Animation::addFrame(const Frame& frame)
{
	mFrames.push_back(frame);
}


void Animation::update(double dt)
{
	mTime += dt;
	auto t = mFrames.front();
	if (t.Time >= mTime)
	{
		mTime -= t.Time;
		mFrames.pop_front();
		mFrames.push_back(t);
	}
}

sf::IntRect Animation::getRect() const
{
	return mFrames.front().TextureRect;
}
