#include <Windows.h> //for FindFirstFile, FindNextFile, FindClose
#include "ScanDir.h"

namespace Util {
    ProcessFileProcedure::ProcessFileProcedure(std::wstring const& directoryName)
    : directoryName_(directoryName) {
    }

    void ProcessFileProcedure::operator()(WIN32_FIND_DATA const& FindFileData) {
        //do nothing
        //override by children-class
    }

    void ProcessFiles(std::wstring const& pattern, ProcessFileProcedure& fileproc) {
        auto findFilename = fileproc.directoryName_ + L"/" + pattern;
        WIN32_FIND_DATA findFileData;
        auto hFind = ::FindFirstFile(findFilename.c_str(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            printf("not find file\n");
        } else {
            bool finished = false;
            do {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { //is directory, recursive
                    auto origDir = fileproc.directoryName_;
                    fileproc.directoryName_ += std::wstring(L"/") + findFileData.cFileName;
                    ProcessFiles(pattern, fileproc);
                    fileproc.directoryName_ = origDir;
                } else {
                    fileproc(findFileData);
                    if (!::FindNextFile(hFind, &findFileData)) {
                        if (::GetLastError() == ERROR_NO_MORE_FILES) { //find end.
                            finished = true;
                        } else {
                            //Couldn't find next file.
                        }
                    }
                }
            } while (!finished);
            ::FindClose(hFind);
        }
    }
}
