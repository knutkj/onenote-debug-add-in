#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "Globals.h"

static void GetLogFilePath(char *logPath, size_t logPathSize)
{
    GetModuleFileNameA(g_hModule, logPath, (DWORD)logPathSize);

    // Find the last dot and replace .dll with .log
    char *lastDot = strrchr(logPath, '.');
    if (lastDot)
    {
        strcpy_s(lastDot, logPathSize - (lastDot - logPath), ".log");
    }
    else
    {
        // Fallback if no extension found
        strcat_s(logPath, logPathSize, ".log");
    }
}

void static GetTimestamp(char *timestamp, size_t timestampSize)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf_s(timestamp, timestampSize, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
              st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

void Log(const char *functionName, const char *message)
{
    char logPath[MAX_PATH];
    char timestamp[32];
    char logEntry[1024];

    GetLogFilePath(logPath, sizeof(logPath));
    GetTimestamp(timestamp, sizeof(timestamp));

    sprintf_s(logEntry, sizeof(logEntry), "[%s]: [%s]: %s\r\n",
              timestamp, functionName, message);

    HANDLE hFile = CreateFileA(
        logPath,
        FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten;
        WriteFile(hFile, logEntry, (DWORD)strlen(logEntry), &bytesWritten, NULL);
        CloseHandle(hFile);
    }
}

void LogProcess()
{
    char logPath[MAX_PATH];
    char processPath[MAX_PATH];
    char timestamp[32];
    char logEntry[1024];
    char *processName;
    DWORD pid;

    GetLogFilePath(logPath, sizeof(logPath));
    GetTimestamp(timestamp, sizeof(timestamp));

    // Get current process ID
    pid = GetCurrentProcessId();

    // Get current process executable path
    GetModuleFileNameA(NULL, processPath, sizeof(processPath));

    // Extract process name from full path
    processName = strrchr(processPath, '\\');
    if (processName)
    {
        processName++; // Skip the backslash
    }
    else
    {
        processName = processPath; // No path separator found
    }

    sprintf_s(logEntry, sizeof(logEntry), "[%s]: [ProcessInfo]: Process=%s PID=%lu Path=%s\r\n",
              timestamp, processName, pid, processPath);

    HANDLE hFile = CreateFileA(
        logPath,
        FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten;
        WriteFile(hFile, logEntry, (DWORD)strlen(logEntry), &bytesWritten, NULL);
        CloseHandle(hFile);
    }
}