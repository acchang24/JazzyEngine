#include "stdafx.h"
#include "Exception.h"

Exception::Exception(int line, const char* file)
	: mLine(line)
	, mFile(file)
{
}

std::wstring Exception::What() const
{
	std::ostringstream oss;
	oss << (char*)GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	std::wstring wide = std::wstring(whatBuffer.begin(), whatBuffer.end());
	return wide;
}

const wchar_t* Exception::GetType() const
{
	return L"Exception Handler";
}

int Exception::GetLine() const
{
	return mLine;
}

const std::string& Exception::GetFile() const
{
	return mFile;
}

std::string Exception::GetOriginString() const
{
	std::ostringstream oss;
	oss << "[File] " << mFile << std::endl << "[Line] " << mLine;
	return oss.str();
}