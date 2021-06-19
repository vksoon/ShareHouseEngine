#pragma once
#include "ShPrerequisites.h"
#include "ShString.h"

SH_NAMESPACE_BEGIN

enum class EAlarm : uint8
{
	Unknown,
	Normal,
	Warning,
	Fatal,
	Error,
};

class OutputSerializer
{
public:
	OutputSerializer() = default;
	OutputSerializer(OutputSerializer&&) = default;
	OutputSerializer(const OutputSerializer&) = default;
	OutputSerializer& operator=(OutputSerializer&&) = default;
	OutputSerializer& operator=(const OutputSerializer&) = default;
	virtual ~OutputSerializer() = default;

	// FOutputDevice interface.
	virtual void Serialize(const String& category, const String& output, EAlarm alarm) = 0;
	virtual void Serialize(const String& category, const String& output, EAlarm alarm, const double time)
	{
		Serialize(category, output, alarm);
	}
	virtual bool Initialize() { return true; }
	virtual void Flush()	  {}
	virtual void TearDown()	  {}
};

class OutputSerializerConsole : public OutputSerializer
{
public:
	virtual bool Show(bool ShowWindow) = 0;
	virtual bool IsShown() = 0;
};

SH_NAMESPACE_END
