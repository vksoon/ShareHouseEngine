#include "ShLog.h"
SH_NAMESPACE_BEGIN

 
void LogOutputDevice::Initialize(LogOutputInitInfo& information)
{
#if _DEBUG || USE_WINDOWS_COSOLE
	if (information.bUseConsoleOutput)
		mOutputSerializer = &WindowsOutputSerializerConsole::GetWindowsConsoleOutput();
	
#endif

	if (mOutputSerializer)
	{
		if (!mOutputSerializer->Initialize())
		{
			// fail to init, start to recover loging system.
			mOutputSerializer = nullptr;
		}
		else
		{
			// recover logic.
		}
	}
}

void LogOutputDevice::TearDown()
{
	if (mOutputSerializer)
		mOutputSerializer->TearDown();
}

SH_NAMESPACE_END
