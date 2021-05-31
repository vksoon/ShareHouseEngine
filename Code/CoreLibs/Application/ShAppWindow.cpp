#include "ShAppWindow.h"
#include "ShApplication.h"
#include "ShWindowFwd.h"

#if SH_PLATFORM == SH_PLATFORM_MSW
#include "Platform/ShApplicationImplMsw.h"
#endif

SH_NAMESPACE_BEGIN

void* AppWindow::GetWindowHandle() const
{
#if SH_PLATFORM == SH_PLATFORM_MSW
	return mImpl->GetWindowHandle();
#endif
}

bool AppWindow::IsFullScreen() const
{
	return mImpl->IsFullScreen();
}

void AppWindow::SetFullScreen(bool fullScreen /* = true */)
{
	mImpl->SetFullScreen(fullScreen);
}


int32 AppWindow::GetWidth() const
{
	return mImpl->GetWidth();
}

int32 AppWindow::GetHeight() const
{
	return mImpl->GetHeight();
}

void AppWindow::SetSize(int32 width, int32 height)
{
	mImpl->SetSize(width, height);
}

int32 AppWindow::GetPosX() const
{
	return mImpl->GetPosX();
}

int32 AppWindow::GetPosY() const
{
	return mImpl->GetPosY();
}

void AppWindow::SetPos(int32 x, int32 y)
{
	mImpl->SetPos(x, y);
}

String AppWindow::GetTitle() const
{
	return mImpl->GetTitle();
}

void AppWindow::SetTitle(const String& title)
{
	mImpl->SetTitle(title);
}

bool AppWindow::IsBorderless() const
{
	return mImpl->IsBorderless();
}

void AppWindow::SetBorderless(bool borderless)
{
	mImpl->SetBorderless(borderless);
}

bool AppWindow::IsAlwaysOnTop() const
{
	return mImpl->IsAlwaysOnTop();
}

void AppWindow::SetAlwaysOnTop(bool alwaysOnTop)
{
	mImpl->SetAlwaysOnTop(alwaysOnTop);
}

void AppWindow::Hide()
{
	mImpl->Hide();
}

void AppWindow::Show()
{
	mImpl->Show();
}

bool AppWindow::IsHidden() const
{
	return mImpl->IsHidden();
}

void AppWindow::Close()
{
	mImpl->Close();
}

void AppWindow::EmitResize()
{
	GetApp()->Resize();
}


SH_NAMESPACE_END