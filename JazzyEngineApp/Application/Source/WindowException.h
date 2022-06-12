#pragma once
#include "Exception.h"

class WindowException : public Exception
{
public:
	WindowException(int line, const char* file, HRESULT result);
	std::wstring What() const override;
	const wchar_t* GetType() const override;
	static std::string TranslateErrorCode(HRESULT result);
	HRESULT GetErrorCode() const;
	std::string GetErrorString() const;
private:
	HRESULT hr;
};

// Helper macro for exceptions
#define WND_EXCEPT(hr) WindowException(__LINE__, __FILE__, hr);
// Macro without error code
#define WND_LAST_EXCEPT() WindowException(__LINE__, __FILE__, GetLastError());