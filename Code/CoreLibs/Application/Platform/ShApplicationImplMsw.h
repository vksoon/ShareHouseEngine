#pragma once

#include "ShPrerequisites.h"
#include "../ShAppWindow.h"

#include "ShApplicationMsw.h"

#include <Windows.h>

SH_NAMESPACE_BEGIN

class Application;
class AppWindowImplMsw;

class SH_CORE_API AppImplMsw
{
public:
	AppImplMsw(Application* app, const AppMsw::Settings& settings);
	~AppImplMsw();

	void Run();
	void Quit();

	Application* GetApp() { return mApp; }
	float GetFrameRate() const { return mFrameRate; }
	void SetFrameRate(float frameRate);

	void DisableFrameRate() { mFrameRateEnabled = false; }
	bool IsFrameRateEnabled() const { return mFrameRateEnabled; }

	UINT32 GetNumWindows() const { return (UINT32)mWindows.size(); }
	WindowRef GetWindowIndex(UINT32 index);
	WindowRef GetForegroundWindow() const { return mForegroundWindow; }

	WindowRef GetActiveWindow() const { return mActiveWindow; }
	void SetActiveWindow(WindowRef window) { mActiveWindow = window; }

private:
	void Sleep(double seconds);
	WindowRef CreateAppWindow(AppWindow::Format format);
	void CloseWindow(AppWindowImplMsw* windowImpl);
	void SetForegroundWindow(WindowRef window) { mForegroundWindow = window; }

	bool SetupHasBeenCalled() const { return mSetupHasBeenCalled; }

private:
	Application* mApp;
	HINSTANCE mInstance;
	WindowRef mActiveWindow;

	float mFrameRate;
	bool mSetupHasBeenCalled;
	bool mNeedsToRefreshDisplays;
	bool mActive;

	double mNextFrameTime;
	bool mFrameRateEnabled;
	bool mShouldQuit;
	bool mQuitOnLastWindowClosed;

	List<AppWindowImplMsw*> mWindows;
	WindowRef mForegroundWindow;

	friend AppMsw;
	friend AppWindowImplMsw;
	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

class SH_CORE_API AppWindowImplMsw
{
public:
	AppWindowImplMsw(const AppWindow::Format& format, AppImplMsw* appImpl);
	~AppWindowImplMsw() {}

	bool IsFullScreen() { return mFullScreen; }
	void SetFullScreen(bool fullScreen);

	int32 GetWidth() const { return mWindowWidth; }
	int32 GetHeight() const { return mWindowHeight; }
	void SetSize(int32 width, int32 height);

	int32 GetPosX() const { return mWindowedPosX; }
	int32 GetPosY() const { return mWindowedPosY; }
	void SetPos(int32 x, int32 y);

	void Close();
	String GetTitle() const;
	void SetTitle(const String& title);

	void Hide();
	void Show();
	bool IsHidden() const;

	bool IsBorderless() const { return mBorderless; }
	void SetBorderless(bool borderless);
	bool IsAlwaysOnTop() const { return mAlwaysOnTop; }
	void SetAlwaysOnTop(bool alwaysOnTop);

	AppImplMsw* GetAppImpl() { return mAppImpl; }
	WindowRef GetWindow() { return mWindow; }
	virtual void Resize();

	HWND GetWindowHandle() const { return mWnd; }
	HDC	GetDC() const { return mDC; }
	void PrivateClose();

private:
	void SetWindowStyleValues();
	void CreateAppWindow(int32 width, int32 height, const String& title);
	void CompleteCreation();
	static void RegisterWindowClass();
	void GetScreenSize(int32 clientWidth, int32 clientHeight, int32* resultWidth, int32* resultHeight);
	virtual void ToggleFullScreen();

private:
	AppImplMsw* mAppImpl;
	WindowRef mWindow;
	HWND mWnd;
	HDC mDC;
	DWORD mWindowStyle;
	DWORD mWindowExStyle;
	int32 mWindowOffsetX;
	int32 mWindowOffsetY;
	bool mHidden;
	int32 mWindowWidth;
	int32 mWindowHeight;
	bool mFullScreen;
	bool mBorderless;
	bool mAlwaysOnTop;
	bool mResizable;
	int32 mWindowedPosX;
	int32 mWindowedPosY;

	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

SH_NAMESPACE_END