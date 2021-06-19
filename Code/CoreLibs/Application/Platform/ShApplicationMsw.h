#pragma once

#include "ShPrerequisites.h"
#include "../ShApplication.h"
#include "../ShWindowFwd.h"

SH_NAMESPACE_BEGIN

class AppImplMsw;

class SH_CORE_API AppMsw
	: public Application
{
public:
	typedef std::function<void(Settings* settings)> AppSettings;

public:
	AppMsw();
	virtual ~AppMsw();

	virtual WindowRef CreateAppWindow(const AppWindow::Format& format = AppWindow::Format()) override;
	virtual WindowRef GetActiveWindow() const override;
	virtual void Quit() override;

	virtual float GetFrameRate() const override;
	virtual void SetFrameRate(float frameRate) override;
	virtual void DisableFrameRate() override;
	virtual bool IsFrameRateEnabled() const override;

private:
	virtual void Launch() override;

private:
	SharedPtr<AppImplMsw> mImpl;

public:
	template<typename _AppType>
	static void Main(const char* title, const AppSettings& settings = AppSettings());
};

template<typename _AppType>
void AppMsw::Main(const char* title, const AppSettings& settingsFunc)
{
	Application::PrepareLaunch();

	Settings entrySettings;

	if (settingsFunc)
		settingsFunc(&entrySettings);

	Application::SetupLaunch(&entrySettings, title, 0, nullptr);

	if (entrySettings.ShouldQuit())
		return;

	AppMsw* app = SH_New<AppMsw>();
	app->ExecuteLaunch();
	SH_Delete(app);

	Application::CleanupLaunch();
}

SH_NAMESPACE_END

#define SH_APP_LAUNCH_MSW( APP, Title, ... )																					\
int32_t CALLBACK WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ) \
{																														\
	ShareHouse::AppMsw::Main<APP>( Title, ##__VA_ARGS__ );															\
	return 0;																											\
}