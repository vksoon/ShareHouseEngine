#pragma once
#include "ShPrerequisites.h"
#include "ShOutputSerializer.h"
#include "ShMessage.h"
#include "Application/Platform/ShWindowsOutputSerializerConsole.h"

SH_NAMESPACE_BEGIN

class LogOutputDevice
{
	OutputSerializer* mOutputSerializer = nullptr;

public:
	LogOutputDevice() = default;
	LogOutputDevice(LogOutputDevice&&) = default;
	LogOutputDevice(const LogOutputDevice&) = default;
	LogOutputDevice& operator=(LogOutputDevice&&) = default;
	LogOutputDevice& operator=(const LogOutputDevice&) = default;
	virtual ~LogOutputDevice() = default;

	struct LogOutputInitInfo
	{
		bool bUseConsoleOutput;
	};

	void Initialize(LogOutputInitInfo&);
	void TearDown();

	bool IsValid() const { return mOutputSerializer != nullptr;  }
	OutputSerializer* GetSerializer() { return mOutputSerializer; }

	static LogOutputDevice& GetLogger()
	{
		static LogOutputDevice Logger;
		return Logger;
	}

	
};

#define SH_LOG(category, alarm, fmt, ...)\
		ShareHouse::Message::Log(String(#category ), EAlarm::alarm, fmt,  ##__VA_ARGS__);\

SH_NAMESPACE_END
