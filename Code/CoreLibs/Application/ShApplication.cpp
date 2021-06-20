#include "ShApplication.h"
#include "Foundation/ShLog.h"
SH_NAMESPACE_BEGIN

Application::Settings* Application::StartUpSettings;

/// <summary>
/// Launcher
/// </summary>

void Application::PrepareLaunch()
{
}

void Application::SetupLaunch(Settings* settings, const char* title, int32 argc, char* const argv[])
{
	StartUpSettings = settings;

	if (settings->GetTitle().empty())
	{
		settings->SetTitle(title);
	}

	if (settings->mUseLogSystem)
	{
		LogOutputDevice::LogOutputInitInfo info;
#if USE_WINDOWS_COSOLE
		info.bUseConsoleOutput = true;
#endif
		LogOutputDevice::GetLogger().Initialize(info);
	}
}

void Application::ExecuteLaunch()
{
	try
	{
		if (mQuitRequested)
			return;

		mLaunchCalled = true;
		Launch();
	}
	catch (std::exception& exc)
	{
		throw;
	}
}

void Application::CleanupLaunch()
{
	LogOutputDevice::GetLogger().TearDown();

}

Application::Application()
	: mFrameCount(0)
	, mAverageFps(0)
	, mFpsSampleInterval(1)
	, mFpsLastSampleFrame(0)
	, mFpsLastSampleTime(0)
	, mLaunchCalled(false)
	, mQuitRequested(false)
{
}

Application::~Application()
{
}

void Application::PrivateSetup()
{
	Setup();
}

void Application::PrivateUpdate()
{
	mFrameCount++;

	Update();
	Draw();

	double now = (double)mTimer.GetSeconds();
	if (now > mFpsLastSampleTime + mFpsSampleInterval)
	{
		//calculate average Fps over sample interval
		UINT32 framesPassed = mFrameCount - mFpsLastSampleFrame;
		mAverageFps = (float)(framesPassed / (now - mFpsLastSampleTime));

		mFpsLastSampleTime = now;
		mFpsLastSampleFrame = mFrameCount;
	}
}

void Application::Update()
{

}

void Application::Draw()
{

}

double Application::GetElapsedSeconds() const
{
	return (double)mTimer.GetSeconds();
}

void Application::EmitCleanup()
{
	CleanUp();
}

//////////////////////////////////////////////////////////////////////////

Application::AppTimer::AppTimer()
{
	Reset();
}

void Application::AppTimer::Reset()
{
	mStartTime = mHRClock.now();
}

uint64 Application::AppTimer::GetSeconds() const
{
	auto newTime = mHRClock.now();
	std::chrono::duration<double> d = newTime - mStartTime;

	return std::chrono::duration_cast<std::chrono::seconds>(d).count();
}

uint64 Application::AppTimer::GetMilliSeconds() const
{
	auto newTime = mHRClock.now();
	std::chrono::duration<double> d = newTime - mStartTime;

	return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

uint64 Application::AppTimer::GetMicroSeconds() const
{
	auto newTime = mHRClock.now();
	std::chrono::duration<double> d = newTime - mStartTime;

	return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

uint64 Application::AppTimer::GetStartMilliSeconds() const
{
	std::chrono::nanoseconds startTimeNs = mStartTime.time_since_epoch();

	return std::chrono::duration_cast<std::chrono::microseconds>(startTimeNs).count();
}

SH_NAMESPACE_END