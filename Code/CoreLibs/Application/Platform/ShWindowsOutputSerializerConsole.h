#pragma once
#include "ShPrerequisites.h"
#include "ShWindowsApi.h"
#include "Foundation/ShOutputSerializer.h"
///////////////////////////////////////////////////////////////////////////////
//	Sh Windows output serializer console
///////////////////////////////////////////////////////////////////////////////
#define USE_WINDOWS_COSOLE 1

#ifdef USE_WINDOWS_COSOLE

SH_NAMESPACE_BEGIN

class WindowsOutputSerializerConsole : public OutputSerializerConsole
{
private:
	/** Handle to the console log window */
	Windows::HANDLE mConsoleHandle;

	void SetColor(const TCHAR* color);

	void SetColor(EAlarm category);
public:

	WindowsOutputSerializerConsole()
		: mConsoleHandle(0)
	{
	}


	// FOutputDevice interface.
	virtual void Serialize(const String& category, const String& output, EAlarm alarm) override;
	virtual void Serialize(const String& category, const String& output, EAlarm alarm, const double Time)override;

	virtual bool Show(bool showWindow);

	virtual bool IsShown() override;

	virtual bool Initialize() override
	{
		return Show(true);
	}

	virtual void TearDown() override
	{
		Show(false);
	}

	static OutputSerializerConsole& GetWindowsConsoleOutput()
	{
		static WindowsOutputSerializerConsole ConsoleOutput;
		return ConsoleOutput;
	}
};

SH_NAMESPACE_END

SH_NAMESPACE_BEGIN_EX(OutputDeviceColor)

const TCHAR* const colorBlack = TEXT("0000");
const TCHAR* const colorRed = TEXT("1001");
const TCHAR* const colorGreen = TEXT("0101");
const TCHAR* const colorBlue = TEXT("0011");
const TCHAR* const colorYellow = TEXT("1101");
const TCHAR* const colorCyan = TEXT("0111");
const TCHAR* const colorPurple = TEXT("1011");
const TCHAR* const colorWhite = TEXT("1111");

const TCHAR* const colorNone = TEXT("");

SH_NAMESPACE_END_EX

#endif