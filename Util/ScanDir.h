#ifndef __UTIL_SCANDIR_H__
#define __UTIL_SCANDIR_H__

#include <winbase.h> //for WIN32_FIND_DATA
#include <string> //for std::wstring

namespace Util {
    class ProcessFileProcedure {
        friend void ProcessFiles(std::wstring const& pattern, ProcessFileProcedure& fileproc);
    public:
        ProcessFileProcedure(std::wstring const& directoryName);
    private:
        virtual void operator()(WIN32_FIND_DATA const& FindFileData);
    protected:
        std::wstring directoryName_;
    };

    void ProcessFiles(std::wstring const& pattern, ProcessFileProcedure& fileproc);
}

#endif //__UTIL_SCANDIR_H__
