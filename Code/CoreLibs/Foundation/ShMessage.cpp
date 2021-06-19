#include "ShMessage.h"
SH_NAMESPACE_BEGIN

int32 Message::GetVarArgs(char* dest, size_t destSize, const char*& fmt, va_list argPtr)
{
	int32 Result = vsnprintf(dest, destSize, fmt, argPtr);
	va_end(argPtr);
	return (Result != -1 && Result < (int32)destSize) ? Result : -1;
}


String Message::Print(const char* fmt, ...)
{
	int32 bufferSize = 512;
	char startingBuffer[512];
	char* buffer = startingBuffer;

	va_list ap;
	va_start(ap, fmt);
	int32 result = Message::GetVarArgs(buffer, bufferSize, fmt, ap);
	if (result >= bufferSize)
	{
		result = -1;
	}
	va_end(ap);

	if (result != -1)
	{
		buffer[result] = 0;
		return String(buffer);
	}
	return String("Exceed Maximum supported buffer\n");
}

SH_NAMESPACE_END