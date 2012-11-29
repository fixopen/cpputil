#include <fstream> //for std::fstream
#include <cstdio> //for std::FILE, std::fopen, std::fclose, ...

#include "IniFile.h"

namespace Util {
    using std::string;
    using std::map;
    using std::vector;
    using std::pair;

    IniFile::IniFile(string const& filename)
        : isDirty_(false)
        , isSaved_(false) {
        if (filename != "") {
            Load(filename);
        }
    }

    IniFile::~IniFile() {
        if (isDirty_) {
            if (!isSaved_) {
                Save(filename_);
            }
        }
    }

    void IniFile::Load(string const& filename) {
        filename_ = filename;
        FILE* file = fopen(filename.c_str(), "r");

        if (file) {
            string sectionName;
            vector<pair<string, string> > properties;
            pair<string, vector<pair<string, string> > > section;
            char line[4096];

            while (!feof(file)) {
                memset(line, 0, 4096);
                fgets(line, 4096, file);
                auto length = strlen(line);

                if (length == 0) {
                    continue;
                }

                auto back = line + length;

                while (isspace(*--back)) {
                    ;
                }

                *(back + 1) = 0;
                length = strlen(line);

                if (length == 0 || length == 1) { //empty line, only \n(newline)
                    continue;
                }

                if ((line[0] == '[') && (line[length - 1] == ']')) {
                    if (sectionName != "") {
                        section = std::make_pair(sectionName, properties);
                        configs_.push_back(section);
                        properties.clear();
                    }

                    sectionName = string(line + 1, length - 2);
                } else {
                    auto v = strchr(line, '=');

                    if (!v) {
                        continue;
                    }

                    auto propertyNameLength = v - line;
                    auto propertyName = string(line, propertyNameLength);
                    ++v;
                    auto propertyValueLength = length - propertyNameLength - 1;
                    auto propertyValue = string(v, propertyValueLength);
                    properties.push_back(std::make_pair(propertyName, propertyValue));
                }
            }

            if (sectionName != "") {
                section = std::make_pair(sectionName, properties);
                configs_.push_back(section);
                properties.clear();
            }

            fclose(file);
        }
    }

    void IniFile::Save(string const& filename) const {
        FILE* file = 0;

        if (filename != "") {
            file = fopen(filename.c_str(), "w");
        } else {
            file = fopen(filename_.c_str(), "w");
        }

        if (file) {
            for (auto i = configs_.begin(), ei = configs_.end(); i != ei; ++i) {
                fputs(('[' + i->first + "]\n").c_str(), file);

                for (auto j = i->second.begin(), ej = i->second.end(); j != ej; ++j) {
                    fputs((j->first + "=" + j->second + '\n').c_str(), file);
                }
            }

            fclose(file);
            isSaved_ = true;
            isDirty_ = false;
        }
    }

    bool const IniFile::HasSection(std::string const& sectionName) const {
        bool result = false;

        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                result = true;
                break;
            }
        }

        return result;
    }

    void IniFile::AppendSection(string const& sectionName, vector<pair<string, string> > const& properties) {
        configs_.push_back(std::make_pair(sectionName, properties));
        isDirty_ = true;
    }

    void IniFile::RemoveSection(string const& sectionName) {
        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                configs_.erase(i);
                isDirty_ = true;
                break;
            }
        }
    }

    vector<pair<string, string> > const IniFile::GetSectionProperties(string const& sectionName) const {
        vector<pair<string, string> > result;

        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                result = i->second;
                break;
            }
        }

        return result;
    }

    bool const IniFile::HasProperty(std::string const& sectionName, std::string const& propertyName) const {
        bool result = false;

        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                for (auto j = i->second.begin(), e = i->second.end(); j != e; ++j) {
                    if (j->first == propertyName) {
                        result = true;
                        break;
                    }
                }

                break;
            }
        }

        return result;
    }

    void IniFile::AppendProperty(string const& sectionName, string const& name, string const& value) {
        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                i->second.push_back(std::make_pair(name, value));
                isDirty_ = true;
                break;
            }
        }
    }

    void IniFile::RemoveProperty(string const& sectionName, string const& propertyName) {
        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                for (auto j = i->second.begin(), e = i->second.end(); j != e; ++j) {
                    if (j->first == propertyName) {
                        i->second.erase(j);
                        isDirty_ = true;
                        break;
                    }
                }

                break;
            }
        }
    }

    string const IniFile::GetPropertyValue(string const& sectionName, string const& propertyName) const {
        string result;

        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                for (auto j = i->second.begin(), e = i->second.end(); j != e; ++j) {
                    if (j->first == propertyName) {
                        result = j->second;
                        break;
                    }
                }

                break;
            }
        }

        return result;
    }

    void IniFile::SetPropertyValue(string const& sectionName, string const& propertyName, string const& newValue) {
        for (auto i = configs_.begin(), e = configs_.end(); i != e; ++i) {
            if (i->first == sectionName) {
                for (auto j = i->second.begin(), e = i->second.end(); j != e; ++j) {
                    if (j->first == propertyName) {
                        j->second = newValue;
                        isDirty_ = true;
                        break;
                    }
                }

                break;
            }
        }
    }
}

