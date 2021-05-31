#include "Application/ShApplicationEntry.h"

using namespace ShareHouse;

void PrepareSettings(App::Settings* settings)
{
	// Window 
	settings->SetWindowPos(200, 200);
	settings->SetWindowSize(1280, 720);
	settings->SetFrameRate(60.0f);
	settings->SetResizable();
	settings->SetFullScreen(false);
}

SHAREHOUSE_APP_ENTRY(App, "ShareHouse App", PrepareSettings)