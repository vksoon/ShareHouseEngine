#pragma once
#include "ShPrerequisites.h"

SH_NAMESPACE_BEGIN

enum class EExceptionType : uint8
{
	Unkown,
	FileNotFound,
	GraphicsKernelException,
};

static const char* GetExceptionTypeDesc(EExceptionType type)
{
	switch (type)
	{
	case EExceptionType::FileNotFound :
		return "FileNotFound";
	case EExceptionType::GraphicsKernelException :
		return "GraphicsKernelException";
	default :
		return "Unkown";
	}
}

template< EExceptionType _Type >
class ExceptionHandler : public std::exception
{
public :
	ExceptionHandler(const String& desc, const String& source)
		: mTypeName(GetExceptionTypeDesc(_Type))
		, mDescription(desc)
		, mSource(source)
	{
	}

	ExceptionHandler(const String& desc, const String& source, const char* file, long line)
		: mLine(line)
		, mTypeName(GetExceptionTypeDesc(_Type))
		, mDescription(desc)
		, mSource(source)
		, mFile(file)
	{
	}

	ExceptionHandler(const ExceptionHandler& rhs)
		: mLine(rhs.mLine)
		, mTypeName(rhs.mTypeName)
		, mDescription(rhs.mDescription)
		, mSource(rhs.mSource)
		, mFile(rhs.mFile)
	{
	}

	virtual const String& GetFullDescription() const
	{
		if (mFullDescription.empty())
		{
			StringStream desc;

			desc << "ShareHouse EXCEPTION(" << mTypeName << "): "
				<< mDescription
				<< " in " << mSource;

			if (mLine > 0)
			{
				desc << " at " << mFile << " (line " << mLine << ")";
			}

			mFullDescription = desc.str();
		}

		return mFullDescription;
	}

	virtual const String& GetSource() const { return mSource; }
	virtual const String& GetFile() const { return mFile; }
	virtual long GetLine() const { return mLine; }
	virtual const String& GetDescription() const { return mDescription; }

	// implemented from std::exception
	virtual const char* what() const noexcept override { return GetFullDescription().c_str(); }

protected :
	long mLine = 0;
	String mTypeName;
	String mDescription;
	String mSource;
	String mFile;
	mutable String mFullDescription;
};

class FileNotFoundException : public ExceptionHandler<EExceptionType::FileNotFound> {};
class GraphicsKernelException : public ExceptionHandler<EExceptionType::GraphicsKernelException> {};

#ifndef SH_EXCEPT																			
#define SH_EXCEPT(type, desc)																\
	{																						\
		static_assert((std::is_base_of<ShareHouse::ExceptionHandler, type>::value),			\
			"Invalid exception type (" #type ") for SH_EXCEPT macro."						\
			" It needs to derive from ShareHouse::ExceptionHandler.");						\
	}																					
#endif


SH_NAMESPACE_END