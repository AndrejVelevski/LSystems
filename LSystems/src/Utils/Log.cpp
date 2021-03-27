#include "Log.h"

#include <stdio.h>
#include <string>
#include <windows.h>
#include <mutex>

static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
static std::mutex mutex;

void Log::trace(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	writeLog("[TRACE]: ", message, args, 11);
	va_end(args);
}

void Log::info(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	writeLog("[INFO]: ", message, args, 15);
	va_end(args);
}

void Log::success(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	writeLog("[SUCCESS]: ", message, args, 10);
	va_end(args);
}

void Log::warning(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	writeLog("[WARNING]: ", message, args, 14);
	va_end(args);
}

void Log::error(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	writeLog("[ERROR]: ", message, args, 13);
	va_end(args);
}

void Log::fatal(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	writeLog("[FATAL]: ", message, args, 12);
	va_end(args);
}

void Log::writeLog(const char* prepend, const char* message, va_list args, int color)
{
	mutex.lock();
	SetConsoleTextAttribute(console, color);
	vprintf((std::string(prepend) + message + "\n").c_str(), args);
	mutex.unlock();
}
