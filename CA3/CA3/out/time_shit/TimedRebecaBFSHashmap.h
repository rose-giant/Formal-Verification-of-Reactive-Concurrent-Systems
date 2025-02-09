

#ifndef __TIMED_REBECA_BFS_HASHMAP_H__
#define __TIMED_REBECA_BFS_HASHMAP_H__

#include "Types.h"
#include "Config.h"
#include "BFSHashmapTemplate.h"
#include "AbstractTimedActor.h"

struct RebecsShiftTime {
	TIME_TYPE shift[REBEC_COUNT];
};

class TimedParentReference;

class TimedBFSState : public BFSState<TimedRebecStateIndexType, TimedParentReference> {
public:
    vector<RebecsShiftTime> shiftTimes;
};

class TimedParentReference {
public:
	TimedBFSState* parent;
	byte executedRebecIndex;
	int timeBundleIndex;
};


class TimedRebecaBFSHashmap : public BFSHashmapTemplate<TimedBFSState, TimedParentReference> {
public:
	TimedRebecaBFSHashmap(int hashmapSize, ostream &out) : BFSHashmapTemplate<TimedBFSState, TimedParentReference>(hashmapSize, out){}
	virtual ~TimedRebecaBFSHashmap() {}

    TimedBFSState* put(TimedBFSState& state, TimedBFSState* parent, int executedRebecIndex, RebecsShiftTime &shiftTimes) {
		TimedBFSState* retValue = BFSHashmapTemplate<TimedBFSState, TimedParentReference>::put(state, parent, executedRebecIndex, 0);
		retValue->shiftTimes.push_back(shiftTimes);
		return retValue;
    }	
};

#endif