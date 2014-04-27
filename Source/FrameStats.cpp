#include "FrameStats.hpp"

#include <algorithm>


FrameStats::FrameStats() : mCurFPS(0), mSmoothFactor(Smooth_None)
{
	mFrameTimes.push_back(std::chrono::high_resolution_clock::duration());
	mFPSes.push_back(0);
}

FrameStats::FrameStats(const FrameStats& rhs) : mFrameTimes(rhs.mFrameTimes), mFPSes(rhs.mFPSes), mCurFPS(rhs.mCurFPS), mCurrentTime(rhs.mCurrentTime), mSmoothFactor(rhs.mSmoothFactor)
{

}

FrameStats::FrameStats(FrameStats&& rhs) : mFrameTimes(std::move(rhs.mFrameTimes)), mFPSes(std::move(rhs.mFPSes)), mCurFPS(std::move(rhs.mCurFPS)), mCurrentTime(std::move(rhs.mCurrentTime)), mSmoothFactor(std::move(rhs.mSmoothFactor))
{

}

FrameStats::~FrameStats()
{

}

FrameStats& FrameStats::operator=(FrameStats rhs)
{
	std::swap(mFrameTimes, rhs.mFrameTimes);
	std::swap(mFPSes, rhs.mFPSes);
	std::swap(mCurFPS, rhs.mCurFPS);
	std::swap(mCurrentTime, rhs.mCurrentTime);
	std::swap(mSmoothFactor, rhs.mSmoothFactor);

	return *this;
}

void FrameStats::setSmoothingFactor(Smoothing smooth)
{
	mSmoothFactor = smooth;

	mFrameTimes.resize(smooth);
}

uint32_t FrameStats::getFPS() const
{
	return mFPSes.back();
}

uint32_t FrameStats::getSmoothFPS() const
{
	uint32_t temp = 0;

	std::for_each(mFPSes.begin(), mFPSes.end(), [&temp](uint32_t fps) {
		temp += fps;
	});

	return temp / mFPSes.size();
}

double FrameStats::getFrameTime() const
{
	return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(mFrameTimes.back()).count();
}

double FrameStats::getSmoothFrameTime() const
{
	std::chrono::nanoseconds temp;

	std::for_each(mFrameTimes.begin(), mFrameTimes.end(), [&temp](const std::chrono::nanoseconds& it) {
		temp += it;
	});

	return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(temp / mFrameTimes.size()).count();
}

void FrameStats::frame(std::chrono::nanoseconds duration)
{
	++mCurFPS;
	mCurrentTime += duration;

	if (std::chrono::duration_cast<std::chrono::seconds>(mCurrentTime).count() >= 1)
	{
		mCurrentTime -= std::chrono::seconds(1);

		mFPSes.push_back(mCurFPS);
		while (mFPSes.size() > mSmoothFactor)
			mFPSes.pop_front();

		mCurFPS = 0;
	}

	mFrameTimes.push_back(duration);

	while (mFrameTimes.size() > mSmoothFactor)
		mFrameTimes.pop_front();
}