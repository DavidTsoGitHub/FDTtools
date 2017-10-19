#include "Logger.h"
#include "StandardTypes.h"

#ifdef _WIN32
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

void Logger_Info(const char* componentName, const char* message, ...)
{
    char buffer[256];
    struct timeval elapsedTime;
    struct tm* currentDateTime;

    gettimeofday(&elapsedTime, NULL);
    currentDateTime = localtime(&elapsedTime.tv_sec);

    va_list args;
    va_start (args, message);
    vsprintf(buffer, message, args);

    fprintf(stdout, "%02d:%02d:%02d.%03lu [%s:INFO] %s\n", currentDateTime->tm_hour, currentDateTime->tm_min, currentDateTime->tm_sec, elapsedTime.tv_usec/1000, componentName,  buffer);
    fflush(stdout);
}

void Logger_Error(const char* componentName, const char* message, ...)
{
    char buffer[256];
    struct timeval elapsedTime;
    struct tm* currentDateTime;

    gettimeofday(&elapsedTime, NULL);
    currentDateTime = localtime(&elapsedTime.tv_sec);

    va_list args;
    va_start (args, message);
    vsprintf(buffer, message, args);

    fprintf(stderr, "%02d:%02d:%02d.%03lu [%s:ERROR] %s\n", currentDateTime->tm_hour, currentDateTime->tm_min, currentDateTime->tm_sec, elapsedTime.tv_usec/1000, componentName,  buffer);
    fflush(stderr);
}

#else
void Logger_Info(const char* componentName, const char* message, ...)
{
    // No logging possibilities on target
    UNUSED(componentName);
    UNUSED(message);
}

void Logger_Error(const char* componentName, const char* message, ...)
{
    // No logging possibilities on target
    UNUSED(componentName);
    UNUSED(message);
}
#endif

