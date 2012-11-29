#include "Reg.h"

namespace Util {
    Reg::Reg(HKEY rootKey, LPCTSTR path) {
        if (::RegOpenKeyEx(rootKey, path, 0, KEY_READ | KEY_WRITE, &key_) == ERROR_SUCCESS) {
            state_ = false;
        }

        //DWORD dw;
        //if (::RegCreateKeyEx(rootKey, path, 0L, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &key_, &dw) != ERROR_SUCCESS) {
        //    state_ = false;
        //}
    }

    Reg::~Reg() {
        if (key_) {
            ::RegCloseKey(key_);
        }
    }

    LONG Reg::Write(LPCTSTR name, DWORD value) {
        return ::RegSetValueEx(key_, name, 0L, REG_DWORD, (CONST BYTE*)&value, sizeof(DWORD));
    }

    LONG Reg::Write(LPCTSTR name, LPCTSTR value) {
        return ::RegSetValueEx(key_, name, 0L, REG_SZ, (CONST BYTE*)value, (wcslen(value) + 1) * 2);
    }

    LONG Reg::Write(LPCTSTR name, const BYTE* value, DWORD valueLength) {
        return ::RegSetValueEx(key_, name, 0L, REG_BINARY, value, valueLength);
    }

    LONG Reg::Read(LPCTSTR name, DWORD& value) {
        DWORD type;
        DWORD typeSize = sizeof (DWORD);
        DWORD tempValue;
        LONG result = ::RegQueryValueEx(key_, (LPCTSTR)name, NULL, &type, (BYTE*)&tempValue, &typeSize);

        if (result == ERROR_SUCCESS) {
            value = tempValue;
        }

        return result;
    }

    LONG Reg::Read(LPCTSTR name, LPCTSTR value) {
        DWORD type;
        DWORD typeSize = 200;
        LONG result = ::RegQueryValueEx(key_, (LPCTSTR)name, NULL, &type, (BYTE*)value, &typeSize);

        if (result == ERROR_SUCCESS) {
            //
        }

        return result;
    }

    LONG Reg::Read(LPCTSTR name, BYTE* value, DWORD& valueLength) {
        DWORD type;
        return ::RegQueryValueEx(key_, (LPCTSTR)name, NULL, &type, value, &valueLength);
    }
}
