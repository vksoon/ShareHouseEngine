#include "ShApplicationImplMsw.h"
#include "../ShApplication.h"

#include <windowsx.h>
#include <CommDlg.h>
#include <ShellAPI.h>
#include <Shlobj.h>

#define LOSHORT(l)           ((SHORT)(l))
#define HISHORT(l)           ((SHORT)(((DWORD)(l) >> 16) & 0xFFFF))

SH_NAMESPACE_BEGIN

LRESULT CALLBACK WndProc(HWND mWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static const char* WINDOWED_WIN_CLASS_NAME = "ShareHouseWndClass";

AppImplMsw::AppImplMsw(Application* app, const AppMsw::Settings& settings)
	: mApp(app)
	, mSetupHasBeenCalled(false)
	, mActive(true)
	, mNeedsToRefreshDisplays(false)
	, mShouldQuit(false)
{
	mFrameRate = settings.GetFrameRate();
	mFrameRateEnabled = settings.IsFrameRateEnabled();
	mQuitOnLastWindowClosed = settings.IsQuitOnLastWindowCloseEnabled();

	auto format = settings.GetDefaultWindowFormat();
	if (!format.IsTitleSpecified())
		format.SetTitle(settings.GetTitle());

	CreateAppWindow(format);
}

AppImplMsw::~AppImplMsw()
{
}

void AppImplMsw::Run()
{
	mApp->PrivateSetup();
	mSetupHasBeenCalled = true;

	for (auto& window : mWindows)
		window->Resize();

	// initialize our next frame time
	mNextFrameTime = mApp->GetElapsedSeconds();

	// inner loop
	while (!mShouldQuit)
	{
		if (mNeedsToRefreshDisplays)
		{
			mNeedsToRefreshDisplays = false;
			for (auto& window : mWindows)
				window->Resize();
		}

		mApp->PrivateUpdate();

		double currentSeconds = mApp->GetElapsedSeconds();
		double secondsPerFrame = 1.0 / mFrameRate;

		double elapsedSeconds = currentSeconds - mNextFrameTime;
		if (elapsedSeconds > 1.0)
		{
			INT32 numSkipFrames = (INT32)(elapsedSeconds / secondsPerFrame);
			mNextFrameTime += (numSkipFrames * secondsPerFrame);
		}

		mNextFrameTime += secondsPerFrame;

		if ((mFrameRateEnabled) && (mNextFrameTime > currentSeconds))
			Sleep(mNextFrameTime - currentSeconds);
		else
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
}


void AppImplMsw::Sleep(double seconds)
{
	static HANDLE timer = ::CreateWaitableTimer(NULL, FALSE, NULL);

	LARGE_INTEGER waitTime;
	waitTime.QuadPart = (LONGLONG)(seconds * -10000000);
	if (waitTime.QuadPart >= 0)
		return;

	if (!::SetWaitableTimer(timer, &waitTime, 0, NULL, NULL, FALSE))
		return;

	DWORD result;
	MSG msg;
	while (!mShouldQuit)
	{
		result = ::MsgWaitForMultipleObjects(1, &timer, false, INFINITE, QS_ALLINPUT);
		if (result == (WAIT_OBJECT_0 + 1))
		{
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else return; 
	}
}

WindowRef AppImplMsw::CreateAppWindow(AppWindow::Format format)
{
	mWindows.push_back(SH_New<AppWindowImplMsw>(format, this));

	if (mSetupHasBeenCalled)
		mWindows.back()->GetWindow()->EmitResize();

	return mWindows.back()->GetWindow();
}

void AppImplMsw::CloseWindow(AppWindowImplMsw* windowImpl)
{
	auto winIt = find(mWindows.begin(), mWindows.end(), windowImpl);
	if (winIt != mWindows.end())
	{
		windowImpl->PrivateClose();
		SH_Delete(windowImpl);
		mWindows.erase(winIt);
	}

	if (mWindows.empty() && mQuitOnLastWindowClosed)
		mShouldQuit = true;
}

WindowRef AppImplMsw::GetWindowIndex(UINT32 index)
{
	if (index >= mWindows.size())
		return WindowRef();

	auto winIt = mWindows.begin();
	std::advance(winIt, index);
	return (*winIt)->GetWindow();
}

void AppImplMsw::Quit()
{
	mShouldQuit = true;
}

void AppImplMsw::SetFrameRate(float frameRate)
{
	mFrameRate = frameRate;
	mFrameRateEnabled = true;
	mNextFrameTime = mApp->GetElapsedSeconds();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AppWindowImplMsw::AppWindowImplMsw(const AppWindow::Format& format, AppImplMsw* appImpl)
	: mAppImpl(appImpl)
	, mHidden(false)
{
	mFullScreen = format.IsFullScreen();
	mResizable = format.IsResizable();
	mAlwaysOnTop = format.IsAlwaysOnTop();
	mBorderless = format.IsBorderless();

	mWindowWidth = format.GetWidth();
	mWindowHeight = format.GetHeight();

	mWindowOffsetX = mWindowedPosX = format.GetPosX();
	mWindowOffsetY = mWindowedPosY = format.GetPosY();

	CreateAppWindow(mWindowWidth, mWindowHeight, format.GetTitle());
	mWindow = AppWindow::PrivateCreate(this, mAppImpl->GetApp());
	CompleteCreation();
}

void AppWindowImplMsw::SetWindowStyleValues()
{
	if (mFullScreen)
	{
		mWindowExStyle = WS_EX_APPWINDOW;					
		mWindowStyle = WS_POPUP;							
	}
	else if (mBorderless)
	{
		mWindowExStyle = WS_EX_APPWINDOW;
		mWindowStyle = WS_POPUP;
	}
	else
	{
		mWindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES;
		mWindowStyle = (mResizable) ? WS_OVERLAPPEDWINDOW
			: (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);
	}
}

void AppWindowImplMsw::CreateAppWindow(int32 width, int32 height, const String& title)
{
	RECT windowRect;

	if (mFullScreen)
	{

	}
	else
	{
		windowRect.left = mWindowedPosX;
		windowRect.right = mWindowedPosX + width;
		windowRect.top = mWindowedPosY;
		windowRect.bottom = mWindowedPosY + height;
	}

	RegisterWindowClass();
	SetWindowStyleValues();
	::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);

	mWnd = ::CreateWindowEx(mWindowExStyle,	
		WINDOWED_WIN_CLASS_NAME,
		title.c_str(),						
		mWindowStyle,						
		windowRect.left, windowRect.top,	
		windowRect.right - windowRect.left,	
		windowRect.bottom - windowRect.top,	
		nullptr,							
		nullptr,							
		::GetModuleHandle(nullptr),
		reinterpret_cast<LPVOID>(this));

	if (mWnd == 0)
	{
		return;
	}

	mDC = ::GetDC(mWnd);
	if (!mDC)
	{
		return;
	}

	if (mAlwaysOnTop)
	{
		::SetWindowLongA(mWnd, GWL_EXSTYLE, ::GetWindowLongA(mWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
		::SetWindowPos(mWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

void AppWindowImplMsw::CompleteCreation()
{
	::DragAcceptFiles(mWnd, TRUE);
	::ShowWindow(mWnd, SW_SHOW);
	::SetForegroundWindow(mWnd);
	::SetFocus(mWnd);
}

void AppWindowImplMsw::RegisterWindowClass()
{
	static bool sRegistered = false;

	if (sRegistered)
		return;

	WNDCLASSEX	wc;
	HMODULE instance = ::GetModuleHandle(NULL);			
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc = WndProc;						
	wc.cbClsExtra = 0;								
	wc.cbWndExtra = 0;								
	wc.hInstance = instance;						
	wc.hIcon = (HICON)::LoadImage(instance, MAKEINTRESOURCE(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE); 
	wc.hIconSm = NULL;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);		
	wc.hbrBackground = NULL;						
	wc.lpszMenuName = NULL;							
	wc.lpszClassName = WINDOWED_WIN_CLASS_NAME;

	if (!::RegisterClassEx(&wc)) 
	{					
		DWORD err = ::GetLastError();
		return;
	}

	sRegistered = true;
}


void AppWindowImplMsw::SetFullScreen(bool fullScreen)
{
	if (mFullScreen != fullScreen)
		ToggleFullScreen();
}

void AppWindowImplMsw::ToggleFullScreen()
{
	bool prevFullScreen = mFullScreen;

	mFullScreen = !mFullScreen;
	SetWindowStyleValues();

	RECT windowRect;
	if (prevFullScreen)
	{
		windowRect.left = mWindowedPosX;
		windowRect.top = mWindowedPosY;
		windowRect.right = mWindowedPosX + mWindowWidth;
		windowRect.bottom = mWindowedPosY + mWindowHeight;
		::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);
	}
	else
	{
		mWindowedPosX = mWindowOffsetX;
		mWindowedPosY = mWindowOffsetY;
	}

	::SetWindowLongA(mWnd, GWL_STYLE, mWindowStyle);
	::SetWindowLongA(mWnd, GWL_EXSTYLE, mWindowExStyle);
	::SetWindowPos(mWnd, HWND_TOP, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOREDRAW);

	::ShowWindow(mWnd, SW_SHOW);
	::SetForegroundWindow(mWnd);
	::SetFocus(mWnd);
}


void AppWindowImplMsw::GetScreenSize(int32 clientWidth, int32 clientHeight, int32* resultWidth, int32* resultHeight)
{
	RECT windowRect;
	windowRect.left = windowRect.top = 0;
	windowRect.right = clientWidth;
	windowRect.bottom = clientHeight;
	::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);
	*resultWidth = windowRect.right - windowRect.left;
	*resultHeight = windowRect.bottom - windowRect.top;
}

void AppWindowImplMsw::SetSize(int32 width, int32 height)
{
	int32 screenWidthPx, screenHeightPx;
	GetScreenSize(width, height, &screenWidthPx, &screenHeightPx);
	::SetWindowPos(mWnd, HWND_TOP, 0, 0, screenWidthPx, screenHeightPx, SWP_NOMOVE | SWP_NOZORDER);
}

void AppWindowImplMsw::SetPos(int32 x, int32 y)
{
	RECT clientArea;
	clientArea.left = x; clientArea.top = y;
	clientArea.right = x + 1; clientArea.bottom = y + 1;
	::AdjustWindowRectEx(&clientArea, mWindowStyle, FALSE, mWindowExStyle);
	::SetWindowPos(mWnd, HWND_TOP, clientArea.left, clientArea.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	POINT upperLeft;
	upperLeft.x = upperLeft.y = 0;
	::ClientToScreen(mWnd, &upperLeft);
	mWindowOffsetX = upperLeft.x;
	mWindowOffsetY = upperLeft.y;
}

void AppWindowImplMsw::Hide()
{
	::ShowWindow(mWnd, SW_HIDE);
	mHidden = true;
}

void AppWindowImplMsw::Show()
{
	::ShowWindow(mWnd, SW_SHOW);
	mHidden = false;
}

bool AppWindowImplMsw::IsHidden() const
{
	return mHidden;
}


void AppWindowImplMsw::Close()
{
	GetAppImpl()->CloseWindow(this);
}

String AppWindowImplMsw::GetTitle() const
{
	int32 numChars = ::GetWindowTextLength(mWnd);
	char* title = (char*)SH_Allocate(sizeof(char) * (numChars + 1));
	::GetWindowText(mWnd, &title[0], numChars + 1);
	title[numChars] = 0;
	return String(title);
}

void AppWindowImplMsw::SetTitle(const String& title)
{
	if (title.empty())
		::SetWindowText(mWnd, "");
	else
		::SetWindowText(mWnd, title.c_str());
}


void AppWindowImplMsw::SetBorderless(bool borderless)
{
	if (mBorderless != borderless)
	{
		mBorderless = borderless;
		if (mBorderless)
		{
			mWindowExStyle = WS_EX_APPWINDOW;
			mWindowStyle = WS_POPUP;
		}
		else
		{
			mWindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES;	
			mWindowStyle = (mResizable) ? WS_OVERLAPPEDWINDOW
				: (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);							
		}

		POINT upperLeft;
		upperLeft.x = upperLeft.y = 0;
		::ClientToScreen(mWnd, &upperLeft);

		RECT windowRect;
		::GetClientRect(mWnd, &windowRect);
		windowRect.left += upperLeft.x; windowRect.right += upperLeft.x;
		windowRect.top += upperLeft.y; windowRect.bottom += upperLeft.y;
		::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);		

		::SetWindowLongA(mWnd, GWL_STYLE, mWindowStyle);
		::SetWindowLongA(mWnd, GWL_EXSTYLE, mWindowExStyle);
		::SetWindowPos(mWnd, HWND_TOP, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOZORDER);
		if (mBorderless)
			::InvalidateRect(0, NULL, TRUE);
	}
}

void AppWindowImplMsw::SetAlwaysOnTop(bool alwaysOnTop)
{
	if (mAlwaysOnTop != alwaysOnTop)
	{
		mAlwaysOnTop = alwaysOnTop;
		LONG oldExStyle = ::GetWindowLongA(mWnd, GWL_EXSTYLE);
		if (mAlwaysOnTop)
		{
			::SetWindowLongA(mWnd, GWL_EXSTYLE, oldExStyle | WS_EX_TOPMOST);
			::SetWindowPos(mWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
		else
		{
			::SetWindowLongA(mWnd, GWL_EXSTYLE, oldExStyle &= (~WS_EX_TOPMOST));
			::SetWindowPos(mWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
	}
}

void AppWindowImplMsw::Resize()
{
	mAppImpl->SetActiveWindow(mWindow);
	mWindow->EmitResize();
}


void AppWindowImplMsw::PrivateClose()
{
	if (mDC)
		::ReleaseDC(mWnd, mDC);

	if (mWnd)
		::DestroyWindow(mWnd);

	mWnd = 0;
}

LRESULT CALLBACK WndProc(
	HWND	mWnd,			
	UINT	uMsg,			
	WPARAM	wParam,			
	LPARAM	lParam)			
{
	AppWindowImplMsw* impl;

	if (uMsg == WM_NCCREATE)
	{
		impl = reinterpret_cast<AppWindowImplMsw*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
		::SetWindowLongPtr(mWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)impl);
	}
	else 
		impl = reinterpret_cast<AppWindowImplMsw*>(::GetWindowLongPtr(mWnd, GWLP_USERDATA));

	if (!impl)
		return DefWindowProc(mWnd, uMsg, wParam, lParam);

	AppImplMsw* appImpl = impl->GetAppImpl();
	appImpl->SetActiveWindow(impl->mWindow);

	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			if (!appImpl->mActive)
			{
				appImpl->mActive = true;
			}
		}
		else
		{
			if (appImpl->mActive)
			{
				appImpl->mActive = false;
			}
		}
		break;
	case WM_ACTIVATE:
		if ((wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE))
			appImpl->SetForegroundWindow(impl->GetWindow());
		break;
	case WM_CLOSE:							
		appImpl->CloseWindow(impl);
		return 0;
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
	}break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
	}break;

	case WM_LBUTTONDOWN:
	{
	}break;

	case WM_RBUTTONDOWN:
	{
	}break;

	case WM_MBUTTONDOWN:
	{
	}break;

	case WM_LBUTTONUP:
	{
	}break;

	case WM_RBUTTONUP:
	{
	}break;

	case WM_MBUTTONUP:
	{
	}break;

	case WM_MOUSEWHEEL:
	{
	}break;

	case WM_KILLFOCUS:
		break;
	case WM_MOUSEMOVE:
	{
	}break;

	case WM_SIZE:
		impl->mWindowWidth = LOWORD(lParam);
		impl->mWindowHeight = HIWORD(lParam);
		if (impl->GetWindow() && impl->mAppImpl->SetupHasBeenCalled())
			impl->GetWindow()->EmitResize();
		return 0;
		break;

	case WM_MOVE:
	{
		impl->mWindowOffsetX = LOSHORT(lParam);
		impl->mWindowOffsetY = HISHORT(lParam);
		return 0;
	}break;

	case WM_PAINT:
		break;

	case WM_DISPLAYCHANGE:
		appImpl->mNeedsToRefreshDisplays = true;
		break;
	case WM_DEVICECHANGE:
		appImpl->mNeedsToRefreshDisplays = true;
		break;
	case 0x02E0 /*WM_DPICHANGED*/:
		if (appImpl->SetupHasBeenCalled()) {
			appImpl->mNeedsToRefreshDisplays = true;
		}
		break;
	}

	return DefWindowProc(mWnd, uMsg, wParam, lParam);
}

SH_NAMESPACE_END