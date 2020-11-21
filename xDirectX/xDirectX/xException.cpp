#include "xException.h"
#include <sstream>

xException::xException(int line, const char * file) noexcept
	:
	line(line),
	file(file)
{
}

const char * xException::what() const noexcept
{
	std::ostringstream oss;
	oss << "[FILE]: " << file << std::endl;
	oss << "[LINE]: " << line;

	msgbuffer = oss.str();

	return msgbuffer.c_str();
}

int xException::GetLine() const noexcept
{
	return line;
}

const std::string & xException::GetFile() const noexcept
{
	return file;
}
