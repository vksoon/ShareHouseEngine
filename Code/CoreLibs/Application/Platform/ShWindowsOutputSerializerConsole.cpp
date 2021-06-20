#include "ShWindowsOutputSerializerConsole.h"

SH_NAMESPACE_BEGIN

String CategoryToString(EAlarm category)
{
	String Output;
	switch (category)
	{
	case ShareHouse::EAlarm::Unknown:
		Output = "Unknown";
		break;
	case ShareHouse::EAlarm::Normal:
		Output = "Normal";
		break;
	case ShareHouse::EAlarm::Warning:
		Output = "Warning";
		break;
	case ShareHouse::EAlarm::Fatal:
		Output = "Fatal";
		break;
	case ShareHouse::EAlarm::Error:
		Output = "Error";
		break;
	default:
		Output = "Default";
		break;
	}

	return Output;
}

void WindowsOutputSerializerConsole::Serialize(const String& category, const String& output, EAlarm alarm)
{
	Serialize(category, output, alarm, 0);
}

void WindowsOutputSerializerConsole::Serialize(const String& category, const String& output, EAlarm alarm, const double time)
{
	// time will be supported after implementing Timer System.

	if (mConsoleHandle)
	{
		SetColor(alarm);
		static uint32 LogLine = 0;

		String recordedTime((  std::to_string(LogLine) + " " + category.c_str() + " ] " + CategoryToString(alarm).c_str()  + " [ Time :  " +  std::to_string(time) + " ] -> ").c_str());
		recordedTime += output;
		recordedTime += "\n";

		uint32 written;
		WriteConsole(mConsoleHandle, recordedTime.c_str(), recordedTime.length(), (::DWORD*) & written, nullptr);
	}
 }

 bool WindowsOutputSerializerConsole::Show(bool showWindow)
 {
	 if (showWindow)
	 {
		 if (!mConsoleHandle)
		 {
			 if (!AllocConsole())
			 {
				 return false;
			 }

			 mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

			 if (mConsoleHandle != INVALID_HANDLE_VALUE)
			 {
				 COORD size;
				 size.X = 160;
				 size.Y = 4000;

				 int32 consoleWidth = 160;
				 int32 consoleHeight = 4000;
				 int32 consolePosX = 0;
				 int32 consolePosY = 0;

				 bool bHasX = false;
				 bool bHasY = false;

				 SetConsoleScreenBufferSize(mConsoleHandle, size);

				 CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

				 // Try to set the window width to match the screen buffer width, so that no manual horizontal scrolling or resizing is necessary
				 if (::GetConsoleScreenBufferInfo(mConsoleHandle, &consoleInfo) != 0)
				 {
					 SMALL_RECT NewConsoleWindowRect = consoleInfo.srWindow;
					 NewConsoleWindowRect.Right = consoleInfo.dwSize.X - 1;
					 ::SetConsoleWindowInfo(mConsoleHandle, true, &NewConsoleWindowRect);
				 }

				 RECT WindowRect;
				 ::GetWindowRect(GetConsoleWindow(), &WindowRect);
				 
				 // use windows pos setting file.
				 ::SetWindowPos(GetConsoleWindow(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOSENDCHANGING | SWP_NOZORDER);
			 }
		 }
	 }
	 else if (mConsoleHandle)
	 {
		 mConsoleHandle = nullptr;
		 FreeConsole();
	 }

	 return true;
 }

 bool WindowsOutputSerializerConsole::IsShown()
 {
	 return mConsoleHandle != nullptr;
 }


 void WindowsOutputSerializerConsole::SetColor(EAlarm category)
 {
	 switch (category)
	 {
	 case ShareHouse::EAlarm::Unknown:
		 SetColor(OutputDeviceColor::colorWhite);
		 break;
	 case ShareHouse::EAlarm::Normal:
		 SetColor(OutputDeviceColor::colorWhite);
		 break;
	 case ShareHouse::EAlarm::Warning:
		 SetColor(OutputDeviceColor::colorYellow);
		 break;
	 case ShareHouse::EAlarm::Fatal:
		 SetColor(OutputDeviceColor::colorRed);
		 break;
	 case ShareHouse::EAlarm::Error:
		 SetColor(OutputDeviceColor::colorGreen);
		 break;
	 default:
		 SetColor(OutputDeviceColor::colorWhite);
		 break;
	 }
 }

 void WindowsOutputSerializerConsole::SetColor(const TCHAR* color)
 {
	 if (std::strcmp(color, TEXT("")) == 0)
	 {
		 SetConsoleTextAttribute(mConsoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	 }
	 else
	 {
		 TCHAR string[9];
		 std::memset(string, 0, sizeof(TCHAR) * 9);
		 std::strncpy(string, color, 9);

		 for (TCHAR* s = string; *s; ++s)
		 {
			 *s -= '0';
		 }

		 SetConsoleTextAttribute(mConsoleHandle,
			 (string[0] ? FOREGROUND_RED : 0) |
			 (string[1] ? FOREGROUND_GREEN : 0) |
			 (string[2] ? FOREGROUND_BLUE : 0) |
			 (string[3] ? FOREGROUND_INTENSITY : 0) |
			 (string[4] ? BACKGROUND_RED : 0) |
			 (string[5] ? BACKGROUND_GREEN : 0) |
			 (string[6] ? BACKGROUND_BLUE : 0) |
			 (string[7] ? BACKGROUND_INTENSITY : 0));
	 }
 }


SH_NAMESPACE_END