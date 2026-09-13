#if _WIN32

#include <windows.h>

#include "messageBox.h"

static std::wstring string_to_wstring(const std::string& str, int codepage = CP_UTF8) {
    if (str.empty()) return std::wstring();
    
    // Calculate required buffer size
    int size = MultiByteToWideChar(codepage, 0, str.c_str(), (int)str.size(), nullptr, 0);
    
    // Allocate and fill buffer
    std::wstring result(size, 0);
    MultiByteToWideChar(codepage, 0, str.c_str(), (int)str.size(), &result[0], size);
    
    return result;
}


bool CreateMessageBox(std::string info, std::string title, int flags, int icon)
{
    if (flags == 0)
    {

        if(icon == 0)
        {

            if (!MessageBoxW(NULL, string_to_wstring(info).c_str(), string_to_wstring(title).c_str(), MB_OK))
            {
                return false;
            }

        }
        else if(icon == 1)
        {

            if (!MessageBoxW(NULL, string_to_wstring(info).c_str(), string_to_wstring(title).c_str(), MB_OK | MB_ICONWARNING))
            {
                return false;
            }

        }

    }

    return true;
}

#endif