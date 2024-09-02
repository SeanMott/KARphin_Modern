#include <cstdarg>  // For va_list, va_start, va_end
#include <cstdio>   // For vprintf

#include "OSError.h"

static void OSReport(const char* msg, ...)
{
  va_list marker;
  va_start(marker, msg);  // Initialize the va_list with the variable arguments
  vprintf(msg, marker);   // Print the message to standard output (cmd/console)
  va_end(marker);         // Clean up the va_list
}

// Wrapper function to match the expected signature
void OSReportWrapper(const Core::CPUThreadGuard&)
{
  // Example usage, replace with the actual message to be printed
  OSReport("This is a wrapped OSReport function!\n");
}
