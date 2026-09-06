#pragma once

#include <string>

#define MESBOX_OK int(0)

#define MESBOX_NOICON int(0)
#define MESBOX_WARNINGICON int(1)

bool CreateMessageBox(std::string info, std::string title, int flags, int icon);