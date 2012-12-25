#ifndef __UTIL_OBSERVER_H__
#define __UTIL_OBSERVER_H__

#include <string>
#include <vector>

namespace Util {
    class Observer {
    public:
        virtual ~Observer() = 0 {}
        virtual void Update(std::string const& data) = 0;
    };
    class Observable {
    public:
        virtual ~Observable() {}
        void RegisterObserver(Observer* const ob) {
            observers_.push_back(ob);
        }
        void Notify(std::string const& data) {
            for (size_t i = 0, c = observers_.size(); i < c; ++i) {
                observers_[i]->Update(data);
            }
        }
    private:
        std::vector<Observer* const> observers_;
    };
}

#endif //__UTIL_OBSERVER_H__
