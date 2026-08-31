#pragma once

#ifdef _WIN32

    #include <Windows.h>

    enum class WindowsVersion
    {
        Unknown,
        Win95,
        Win98,
        WinME,
        WinNT4,
        Win2000,
        WinXP,
        WinVista,
        Win7,
        Win8,
        Win81,
        Win10,
        Win11
    };

    WindowsVersion getWindowsVersion();

#endif