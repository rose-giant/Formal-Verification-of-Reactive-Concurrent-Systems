

#ifndef __ABSTRACT_TIMED_ACTOR_H_
#define __ABSTRACT_TIMED_ACTOR_H_

#include "AbstractActor.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include <limits>

using namespace std;

#define typedSwap(x,y,type) { type __tempvar = x; x = y; y = __tempvar; }









	struct __attribute__((__packed__)) TimeFrame {
	    TIME_TYPE* executionTime;
	    TIME_TYPE* deadline;
	    TIME_TYPE now;
	    #ifdef TTS
	    	TIME_TYPE res;
	    #endif
	};
	
	struct __attribute__((__packed__)) TimedRebecStateIndexType {
		long tableIndex;
		TIME_TYPE bundleIndex;
		bool operator == (const TimedRebecStateIndexType &r2) {
			return this->tableIndex == r2.tableIndex && this->bundleIndex == r2.bundleIndex;
		}
	};

class AbstractTimedActor : virtual public AbstractActor {

public:

    AbstractTimedActor(byte myID, char *myName, int maxQueueLength, int maxParamQueueLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif








		, int stateSize, int hashmapSize
    );
    virtual ~AbstractTimedActor();
    virtual void setNow(TIME_TYPE now);
    TIME_TYPE getNow();

    #ifdef TTS
        int __pc;
        TIME_TYPE __res;
    #endif
    
    

    TIME_TYPE* executionTime;
    TIME_TYPE* deadline;
    TIME_TYPE _ref_now;
    
    void preMessageExecutionInitialization();
    TIME_TYPE _ref_currentMessageArrival, _ref_currentMessageDeadline, _ref_currentMessageWaitingTime;

	virtual void exportStateInXML(ostream &out, string tab);
	virtual void exportQueueContentInXML(ostream &out, string tab);
	
    
protected:
	void addTimedBundles(byte senderId, TIME_TYPE executionTime, TIME_TYPE deadline);

    void timeEnqueue(TIME_TYPE *queue, TIME_TYPE value);
    virtual void applyPolicy(boolean isSendToSelf);
    
    void startTimer(Timer &t);
    void stopTimer(Timer &t);
    TIME_TYPE getTimerValue(Timer &t);









	using AbstractActor::marshalActor;
	using AbstractActor::unmarshalActor;
	public:
	
	    TimedRebecStateIndexType marshalActor(TIME_TYPE& shiftTime);
	    void unmarshalActor(TimedRebecStateIndexType state, TIME_TYPE shiftTime);
		TimedRebecStateIndexType storeLocalState(long stateNum, TIME_TYPE& shiftTime);
		
		
	protected:
		int timeBundleCount;
		virtual vector<TimeFrame>** getHashtableTimeExtension() = 0;

};

#endif