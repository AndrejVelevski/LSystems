#pragma once

#include <stdarg.h>

class Log
{
public:
	static void trace(const char* message, ...);
	static void info(const char* message, ...);
	static void success(const char* message, ...);
	static void warning(const char* message, ...);
	static void error(const char* message, ...);
	static void fatal(const char* message, ...);

private:
	static void writeLog(const char* prepend, const char* message, va_list args);
};

