#pragma once
#include "ShPrerequisites.h"
#include "ShAppWindow.h"

// for simple app timer 
#include <chrono>

SH_NAMESPACE_BEGIN

class SH_CORE_API Application
{
public:
	class SH_CORE_API Settings
	{
	public:
		Settings() = default;
		virtual ~Settings() = default;

		void SetDefaultWindowFormat(const AppWindow::Format& format) { mDefaultWindowFormat = format; }
		AppWindow::Format GetDefaultWindowFormat() const { return mDefaultWindowFormat; }

		void SetWindowSize(int32 width, int32 height) { mDefaultWindowFormat.SetSize(width, height); }
		int32 GetWindowWidth() const { return mDefaultWindowFormat.GetWidth(); }
		int32 GetWindowHeight() const { return mDefaultWindowFormat.GetHeight(); }

		void SetWindowPos(int32 windowPosX, int32 windowPosY) { mDefaultWindowFormat.SetPos(windowPosX, windowPosY); }
		int32 GetWindowPosX() const { return mDefaultWindowFormat.GetPosX(); }
		int32 GetWindowPosY() const { return mDefaultWindowFormat.GetPosY(); }

		void SetWindowPosUnspecified() { mDefaultWindowFormat.SetPosUnspecified(); }
		bool IsWindowPosSpecified() const { return mDefaultWindowFormat.IsPosSpecified(); }

		void SetQuitOnLastWindowCloseEnabled(bool enable = true) { mQuitOnLastWindowClose = enable; }
		bool IsQuitOnLastWindowCloseEnabled() const { return mQuitOnLastWindowClose; }

		void SetFullScreen(bool fullScreen = true) { mDefaultWindowFormat.SetFullScreen(fullScreen); }
		bool IsFullScreen() const { return mDefaultWindowFormat.IsFullScreen(); }

		void SetResizable(bool resizable = true) { mDefaultWindowFormat.SetResizable(resizable); }
		bool IsResizable() const { return mDefaultWindowFormat.IsResizable(); }

		void SetBorderless(bool borderless = true) { mDefaultWindowFormat.SetBorderless(borderless); }
		bool IsBorderless() const { return mDefaultWindowFormat.IsBorderless(); }

		void SetAlwaysOnTop(bool alwaysOnTop = true) { mDefaultWindowFormat.SetAlwaysOnTop(alwaysOnTop); }
		bool IsAlwaysOnTop() const { return mDefaultWindowFormat.IsAlwaysOnTop(); }

		void SetTitle(const String& title) { mTitle = title; mDefaultWindowFormat.SetTitle(title); }
		const String& GetTitle() const { return mTitle; }

		void SetFrameRate(float frameRate) { mFrameRate = frameRate; }
		float GetFrameRate() const { return mFrameRate; }

		void EnableFrameRate() { mFrameRateEnabled = true; }
		void DisableFrameRate() { mFrameRateEnabled = false; }
		bool IsFrameRateEnabled() const { return mFrameRateEnabled; }

		void SetShouldQuit(bool shouldQuit = true) { mShouldQuit = shouldQuit; }
		bool ShouldQuit() const { return mShouldQuit; }

		void SetShouldUseLogSystem(bool UseLogSystem = true) { mUseLogSystem = UseLogSystem; }
		bool ShouldUseLogSystem() const { return mUseLogSystem; }

	protected:
		AppWindow::Format mDefaultWindowFormat;
		String			  mTitle;
		bool			  mFrameRateEnabled = true;
		float			  mFrameRate = 60;
		bool			  mQuitOnLastWindowClose = true;
		bool			  mShouldQuit = false;
		bool			  mUseLogSystem = false;

		friend Application;
	};

public:
	~Application();

	virtual void Setup() {}
	virtual void Update();
	virtual void Draw();
	virtual void Resize() {}
	virtual void CleanUp() {}
	virtual void Quit() = 0;

	virtual WindowRef CreateAppWindow(const AppWindow::Format& format = AppWindow::Format()) = 0;
	virtual WindowRef GetActiveWindow() const = 0;

	virtual float GetFrameRate() const = 0;
	virtual void SetFrameRate(float frameRate) = 0;
	virtual void DisableFrameRate() = 0;
	virtual bool IsFrameRateEnabled() const = 0;
	float GetAverageFps() const { return mAverageFps; }
	double GetFpsSampleInterval() const { return mFpsSampleInterval; }
	void SetFpsSampleInterval(double sampleInterval) { mFpsSampleInterval = sampleInterval; }
	virtual bool IsFullScreen() const { return GetActiveWindow()->IsFullScreen(); }
	virtual void SetFullScreen(bool fullScreen) { GetActiveWindow()->SetFullScreen(fullScreen); }

	double GetElapsedSeconds() const;
	uint32 GetElapsedFrames() const { return mFrameCount; }

	bool GetQuitRequested() const { return mQuitRequested; }
	void SetQuitRequested() { mQuitRequested = true; }

	void EmitCleanup();

	void PrivateSetup();
	void PrivateUpdate();

protected:
	Application();

	// Launcher
protected:
	virtual void Launch() = 0;

	bool GetLaunchCalled() const { return mLaunchCalled; }
	void ExecuteLaunch();

	static void	PrepareLaunch();
	static void	SetupLaunch(Settings* settings, const char* title, int32 argc, char* const argv[]);
	static void CleanupLaunch();

	// AppTimer
protected:
	class AppTimer
	{
	public:
		AppTimer();
		void Reset();
		uint64 GetSeconds() const;
		uint64 GetMilliSeconds() const;
		uint64 GetMicroSeconds() const;
		uint64 GetStartMilliSeconds() const;

	private:
		std::chrono::high_resolution_clock mHRClock;
		std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	};

private:
	AppTimer mTimer;
	uint32 mFrameCount;
	float mAverageFps;
	uint32 mFpsLastSampleFrame;
	double mFpsLastSampleTime;
	double mFpsSampleInterval;
	bool mLaunchCalled;
	bool mQuitRequested;

protected:
	static Settings* StartUpSettings;
};


SH_NAMESPACE_END