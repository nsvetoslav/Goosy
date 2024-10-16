#include <stdio.h>
#include <time.h>
#include <windows.h>

int main()
{
    HMODULE hDLL = LoadLibrary("hackjaggo.dll");
    if (hDLL == NULL)
    {
        printf("Failed to load DLL!\n");
        return -1;
    }

    return 0;
}