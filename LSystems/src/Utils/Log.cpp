#include "Log.h"

#include <stdio.h>
#include <string>
#include <windows.h>

static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void Log::trace(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	SetConsoleTextAttribute(console, 11);
	writeLog("[TRACE]: ", message, args);
	SetConsoleTextAttribute(console, 15);
	va_end(args);
}

void Log::info(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	SetConsoleTextAttribute(console, 15);
	writeLog("[INFO]: ", message, args);
	SetConsoleTextAttribute(console, 15);
	va_end(args);
}

void Log::success(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	SetConsoleTextAttribute(console, 10);
	writeLog("[SUCCESS]: ", message, args);
	SetConsoleTextAttribute(console, 15);
	va_end(args);
}

void Log::warning(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	SetConsoleTextAttribute(console, 14);
	writeLog("[WARNING]: ", message, args);
	SetConsoleTextAttribute(console, 15);
	va_end(args);
}

void Log::error(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	SetConsoleTextAttribute(console, 13);
	writeLog("[ERROR]: ", message, args);
	SetConsoleTextAttribute(console, 15);
	va_end(args);
}

void Log::fatal(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	SetConsoleTextAttribute(console, 12);
	writeLog("[FATAL]: ", message, args);
	SetConsoleTextAttribute(console, 15);
	va_end(args);
}

void Log::writeLog(const char* prepend, const char* message, va_list args)
{
	vprintf((std::string(prepend) + message + "\n").c_str(), args);
}
