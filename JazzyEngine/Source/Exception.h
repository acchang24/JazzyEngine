#pragma once
#include <exception>

class Exception : public std::exception
{
public:
	Exception(int line, const char* file);
	virtual std::wstring What() const;
	virtual const wchar_t* GetType() const;
	int GetLine() const;
	const std::string& GetFile() const;
	std::string GetOriginString() const;

private:
	int mLine;
	std::string mFile;

protected:
	mutable std::string whatBuffer;
};

