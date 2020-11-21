#pragma once
#include <exception>
#include <string>

class xException : public std::exception
{
public:
	xException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
public:
	int line;
	std::string file;
public:
	mutable std::string msgbuffer;							// let children use it
};

