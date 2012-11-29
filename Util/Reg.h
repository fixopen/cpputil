#ifndef __UTIL_REG_H__
#define __UTIL_REG_H__

#include <windows.h>

namespace Util {
    class Reg {
    public:
        Reg(HKEY rootKey, LPCTSTR path);
        ~Reg();
        LONG Read(LPCTSTR name, DWORD& value);
        LONG Read(LPCTSTR name, LPCTSTR value);
        LONG Read(LPCTSTR name, BYTE* value, DWORD& valueLength);
        LONG Write(LPCTSTR name, DWORD value);
        LONG Write(LPCTSTR name, LPCTSTR value);
        LONG Write(LPCTSTR name, const BYTE* value, DWORD valueLength);
    protected:
        HKEY key_;
        bool state_;
    };
}

#endif //__UTIL_REG_H__
