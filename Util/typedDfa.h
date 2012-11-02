#ifndef __UTIL_TYPEDDFA_H__
#define __UTIL_TYPEDDFA_H__

#include <vector> //for std::vector
#include <map> //for std::map

namespace Util {
    template<typename T>
    class Dfa {
    public:
        typedef void (T::*EventHandle)(void* parama);

        void registerRule(int const startState, int const event, int const stopState, EventHandle const action = 0) {
            std::pair<T* const, EventHandle> process = std::make_pair(static_cast<T* const>(this), action); //WinCE not support dynamic_cast, use static_cast instead of it
            std::pair<int, std::pair<T* const, EventHandle> > jumpTable = std::make_pair(stopState, process);
            auto i = rules_.find(startState);
            if (i != rules_.end()) {
                auto j = i->second.find(event);
                if (j != i->second.end()) {
                    //error
                } else {
                    i->second.insert(std::make_pair(event, jumpTable));
                }
            } else {
                std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > firstEventRules;
                firstEventRules.insert(std::make_pair(event, jumpTable));
                rules_.insert(std::make_pair(startState, firstEventRules));
            }
        }

        void fireEvent(int const event, void* const param) {
            auto i = rules_.find(currentState_);
		    if (i != rules_.end()) { //state machine is ok
			    auto j = i->second.find(event);
			    if (j != i->second.end()) { //has valid jump rule
				    auto jumpTable = j->second;
				    currentState_ = jumpTable.first;
				    if (jumpTable.second.second) { //has action
					    (jumpTable.second.first->*jumpTable.second.second)(param);
				    }
			    }
		    }
        }

        void setStartState(int const startState) {
            currentState_ = startState;
        }

        void setEndStates(std::vector<int> const& endStates) {
            endStates_ = endStates;
        }

		bool const isEnd() const {
		    auto result = false;
		    for (size_t i = 0; i < endStates_.size(); ++i) {
			    if (endStates_[i] == currentState_) {
				    result = true;
				    break;
			    }
		    }
		    return result;
        }

        int const getCurrentState() const {
            return currentState_;
        }
    private:
        int currentState_;
        std::vector<int> endStates_;
        std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > > rules_;
    };
}

#endif //__UTIL_TYPEDDFA_H__
