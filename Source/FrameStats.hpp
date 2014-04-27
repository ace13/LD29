#pragma once

#include <list>
#include <chrono>
#include <cstdint>

class FrameStats
{
public:
	enum Smoothing
	{
		Smooth_None = 1,
		Smooth_2x = 2,
		Smooth_4x = 4,
		Smooth_6x = 6,
		Smooth_8x = 8
	};

	FrameStats();
	FrameStats(const FrameStats&);
	FrameStats(FrameStats&&);
	~FrameStats();

	FrameStats& operator=(FrameStats);

	void setSmoothingFactor(Smoothing smooth);

	uint32_t getFPS() const;
	uint32_t getSmoothFPS() const;

	double getFrameTime() const;
	double getSmoothFrameTime() const;

	void frame(std::chrono::nanoseconds duration);

private:
	std::list<std::chrono::nanoseconds> mFrameTimes;
	std::list<uint32_t> mFPSes;

	uint32_t mCurFPS;
	std::chrono::nanoseconds mCurrentTime;

	Smoothing mSmoothFactor;
};