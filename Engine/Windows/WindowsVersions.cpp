#include "WindowsVersion.h"

#ifdef _WIN32

    WindowsVersion getWindowsVersion()
    {
        OSVERSIONINFOEXA os = {};
        os.dwOSVersionInfoSize = sizeof(os);

        if (!GetVersionExA(
            reinterpret_cast<OSVERSIONINFOA*>(&os)))
        {
            return WindowsVersion::Unknown;
        }

        if (os.dwMajorVersion == 10)
        {
            return WindowsVersion::Win10;
        }

        if (os.dwMajorVersion == 6 &&
            os.dwMinorVersion == 3)
        {
            return WindowsVersion::Win81;
        }

        if (os.dwMajorVersion == 6 &&
            os.dwMinorVersion == 2)
        {
            return WindowsVersion::Win8;
        }

        if (os.dwMajorVersion == 6 &&
            os.dwMinorVersion == 1)
        {
            return WindowsVersion::Win7;
        }

        if (os.dwMajorVersion == 6 &&
            os.dwMinorVersion == 0)
        {
            return WindowsVersion::WinVista;
        }

        if (os.dwMajorVersion == 5 &&
            os.dwMinorVersion == 2)
        {
            return WindowsVersion::WinXP;
        }

        if (os.dwMajorVersion == 5 &&
            os.dwMinorVersion == 1)
        {
            return WindowsVersion::WinXP;
        }

        if (os.dwMajorVersion == 5 &&
            os.dwMinorVersion == 0)
        {
            return WindowsVersion::Win2000;
        }

        return WindowsVersion::Unknown;
    }

#endif