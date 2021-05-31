#pragma once
#include "ShPrerequisites.h"

#if SH_PLATFORM == SH_PLATFORM_MSW
#include "Platform/ShApplicationMsw.h"
namespace ShareHouse
{
	typedef AppMsw App;
};
#define SHAREHOUSE_APP_ENTRY( APP, Title, ... )	SH_APP_LAUNCH_MSW( APP, Title, ##__VA_ARGS__ )
#endif