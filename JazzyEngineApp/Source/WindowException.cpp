#include "stdafx.h"
#include "WindowException.h"

WindowException::WindowException(int line, const char* file, HRESULT result)
	: Exception(line, file)
	, hr(result)
{
}

std::wstring WindowException::What() const
{
	std::ostringstream oss;
	oss << "Window Exception" << std::endl << "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	std::wstring ws(whatBuffer.begin(), whatBuffer.end());
	return ws;
}

const wchar_t* WindowException::GetType() const
{
	return L"Window Exception";
}

std::string WindowException::TranslateErrorCode(HRESULT result)
{
	LPWSTR pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&pMsgBuf, 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::wstring ws(pMsgBuf);
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
	std::string s = convert.to_bytes(ws);
	// free windows buffer
	LocalFree(pMsgBuf);
	return s;
}

HRESULT WindowException::GetErrorCode() const
{
	return hr;
}

std::string WindowException::GetErrorString() const
{
	return TranslateErrorCode(hr);
}