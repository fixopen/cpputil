#include "miniDfa.h"

namespace Util {
    void Dfa::registerRule(int const startState, int const event, int const stopState, EventHandle const action) {
        std::pair<int, EventHandle> jumpTable = std::make_pair(stopState, action);
        auto i = rules_.find(startState);
        if (i != rules_.end()) {
            auto j = i->second.find(event);
            if (j != i->second.end()) {
                //error
            } else {
                i->second.insert(std::make_pair(event, jumpTable));
            }
        } else {
            std::map<int, std::pair<int, EventHandle> > firstEventRules;
            firstEventRules.insert(std::make_pair(event, jumpTable));
            rules_.insert(std::make_pair(startState, firstEventRules));
        }
    }

    void Dfa::fireEvent(int const event, void* const param) {
        auto i = rules_.find(currentState_);
		if (i != rules_.end()) { //state machine is ok
			auto j = i->second.find(event);
			if (j != i->second.end()) { //has valid jump rule
				auto jumpTable = j->second;
				currentState_ = jumpTable.first;
				if (jumpTable.second) { //has action
					jumpTable.second(param);
				}
			}
		}
    }

    void Dfa::setStartState(int const startState) {
        currentState_ = startState;
    }

    void Dfa::setEndStates(std::vector<int> const& endStates) {
        endStates_ = endStates;
    }

	bool const Dfa::isEnd() const {
		auto result = false;
		for (auto i = 0u; i < endStates_.size(); ++i) {
			if (endStates_[i] == currentState_) {
				result = true;
				break;
			}
		}
		return result;
	}

    int const Dfa::getCurrentState() const {
        return currentState_;
    }
}
