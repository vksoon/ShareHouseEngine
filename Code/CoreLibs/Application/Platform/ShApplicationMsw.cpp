#include "ShApplicationMsw.h"
#include "ShApplicationImplMsw.h"

SH_NAMESPACE_BEGIN

AppMsw::AppMsw()
{
	const Settings* settings = dynamic_cast<Settings*>(StartUpSettings);
	SH_ASSERT(settings);

	mImpl = SH_MakeShared<AppImplMsw>(this, *settings);
}

AppMsw::~AppMsw()
{

}

void AppMsw::Launch()
{
	mImpl->Run();
}

WindowRef AppMsw::CreateAppWindow(const AppWindow::Format& format)
{
	return mImpl->CreateAppWindow(format);
}

void AppMsw::Quit()
{
	// if launch() has not yet been called, we note the request rather than issue the call to mImpl::quit()
	if (!GetLaunchCalled())
		SetQuitRequested();
	else
		mImpl->Quit();
}

float AppMsw::GetFrameRate() const
{
	return mImpl->GetFrameRate();
}

void AppMsw::SetFrameRate(float frameRate)
{
	mImpl->SetFrameRate(frameRate);
}

void AppMsw::DisableFrameRate()
{
	mImpl->DisableFrameRate();
}

bool AppMsw::IsFrameRateEnabled() const
{
	return mImpl->IsFrameRateEnabled();
}

WindowRef AppMsw::GetActiveWindow() const
{
	return mImpl->GetActiveWindow();
}

SH_NAMESPACE_END