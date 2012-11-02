#ifndef __UTIL_MINIDFA_H__
#define __UTIL_MINIDFA_H__

#include <vector> //for std::vector
#include <map> //for std::map

namespace Util {
    class Dfa {
    public:
        typedef void (*EventHandle)(void* parama);

        void registerRule(int const startState, int const event, int const stopState, EventHandle const action = 0);

        void fireEvent(int const event, void* const param);

        void setStartState(int const startState);

        void setEndStates(std::vector<int> const& endStates);

		bool const isEnd() const;

        int const getCurrentState() const;
    private:
        int currentState_;
        std::vector<int> endStates_;
        std::map<int, std::map<int, std::pair<int, EventHandle> > > rules_;
    };
}

#endif //__UTIL_MINIDFA_H__
