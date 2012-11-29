#ifndef __UTIL_INIFILE_H__
#define __UTIL_INIFILE_H__

#include <map> //for std::map
#include <vector> //for std::vector
#include <string> //for std::string, std::wstring

namespace Util {
    class IniFile {
    public:
        explicit IniFile(std::string const& filename = "");
        ~IniFile();
        void Load(std::string const& filename);
        void Save(std::string const& filename = "") const;
        bool const HasSection(std::string const& sectionName) const;
        void AppendSection(std::string const& sectionName, std::vector<std::pair<std::string, std::string> > const& properties);
        void RemoveSection(std::string const& sectionName);
        std::vector<std::pair<std::string, std::string> > const GetSectionProperties(std::string const& sectionName) const;
        bool const HasProperty(std::string const& sectionName, std::string const& propertyName) const;
        void AppendProperty(std::string const& sectionName, std::string const& name, std::string const& value);
        void RemoveProperty(std::string const& sectionName, std::string const& propertyName);
        std::string const GetPropertyValue(std::string const& sectionName, std::string const& propertyName) const;
        void SetPropertyValue(std::string const& sectionName, std::string const& propertyName, std::string const& newValue);
    private:
        std::string filename_;
        mutable bool isDirty_;
        mutable bool isSaved_;
        std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string> > > > configs_;
    };
}

#endif //__UTIL_INIFILE_H__
