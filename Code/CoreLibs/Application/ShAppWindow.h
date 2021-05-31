#pragma once

#include "ShPrerequisites.h"

SH_NAMESPACE_BEGIN

#if SH_PLATFORM == SH_PLATFORM_MSW
class AppWindowImplMsw;
#endif

class Application;
class AppWindow;
typedef SharedPtr<AppWindow> WindowRef;

class SH_CORE_API AppWindow
	: public std::enable_shared_from_this<AppWindow>
{
public:
	struct Format
	{
		Format(bool bFullscreen = false, int32 width = 1280, int32 height = 720, int32 posX = 0, int32 posY = 0)
			: mFullScreen(bFullscreen)
			, mWidth(width)
			, mHeight(height)
			, mPosX(posX)
			, mPosY(posY)
			, mPosSpecified(false)
			, mResizable(true)
			, mBorderless(false)
			, mAlwaysOnTop(false)
			, mFullScreenButtonEnabled(false)
			, mTitleSpecified(false)
			, mTitle("")
		{}

		bool IsFullScreen() const { return mFullScreen; }
		void SetFullScreen(bool bFullScreen) { mFullScreen = bFullScreen; }

		int32 GetWidth() const { return mWidth; }
		int32 GetHeight() const { return mHeight; }
		void SetSize(int32 width, int32 height) { mWidth = width; mHeight = height; }

		int32 GetPosX() const { return mPosX; }
		int32 GetPosY() const { return mPosY; }
		void SetPos(int32 X, int32 Y) { mPosX = X; mPosY = Y; mPosSpecified = true; }
		bool IsPosSpecified() const { return mPosSpecified; }
		void SetPosUnspecified() { mPosSpecified = false; }

		bool IsResizable() const { return mResizable; }
		void SetResizable(bool resizable = true) { mResizable = resizable; }

		bool IsBorderless() const { return mBorderless; }
		void SetBorderless(bool borderless = true) { mBorderless = borderless; }

		bool IsAlwaysOnTop() const { return mAlwaysOnTop; }
		void SetAlwaysOnTop(bool alwaysOnTop = true) { mAlwaysOnTop = alwaysOnTop; }

		bool IsFullScreenButtonEnabled() const { return mFullScreenButtonEnabled; }
		void EnableFullScreenButton(bool enabled = true) { mFullScreenButtonEnabled = enabled; }

		const String& GetTitle() const { return mTitle; }
		void SetTitle(const String& title) { mTitle = title; mTitleSpecified = true; }
		bool IsTitleSpecified() const { return mTitleSpecified; }
		void SetTitleUnspecified() { mTitleSpecified = false; }

	private:
		bool		mFullScreen;
		int32		mWidth;
		int32		mHeight;
		int32		mPosX;
		int32		mPosY;
		bool		mPosSpecified;
		bool		mResizable;
		bool		mBorderless;
		bool		mAlwaysOnTop;
		bool		mFullScreenButtonEnabled;
		String		mTitle;
		bool		mTitleSpecified;
	};

	AppWindow()
		: mApp(nullptr)
		, mImpl(nullptr)
	{
	}

	virtual ~AppWindow() = default;

	Application* GetApp() const { return mApp; }

	void* GetWindowHandle() const;

	bool IsFullScreen() const;
	void SetFullScreen(bool fullScreen = true);

	int32 GetWidth() const;
	int32 GetHeight() const;
	float GetAspectRatio() const { return GetWidth() / (float)GetHeight(); }
	void SetSize(int32 width, int32 height);

	int32 GetPosX() const;
	int32 GetPosY() const;
	void SetPos(int32 x, int32 y);

	float GetCenterX() const { return (float)GetWidth() / 2.0f; }
	float GetCenterY() const { return (float)GetHeight() / 2.0f; }

	String GetTitle() const;
	void SetTitle(const String& title);

	bool IsBorderless() const;
	void SetBorderless(bool borderless = true);
	bool IsAlwaysOnTop() const;
	void SetAlwaysOnTop(bool alwaysOnTop = true);

	void Hide();
	void Show();
	bool IsHidden() const;

	void Close();

	// Window Events
public:
	void EmitResize();

public:
#if SH_PLATFORM == SH_PLATFORM_MSW
	static WindowRef PrivateCreate(AppWindowImplMsw* impl, Application* app)
#endif
	{
		WindowRef ret = SH_MakeShared<AppWindow>();
		ret->SetImpl(impl);
		ret->SetApp(app);

		return ret;
	}

private:
	void SetApp(Application* app) { mApp = app; }
#if SH_PLATFORM == SH_PLATFORM_MSW
	void SetImpl(AppWindowImplMsw* impl) { mImpl = impl; }
#endif


protected:
	Application* mApp;
#if SH_PLATFORM == SH_PLATFORM_MSW
	AppWindowImplMsw* mImpl;
#endif
};

SH_NAMESPACE_END