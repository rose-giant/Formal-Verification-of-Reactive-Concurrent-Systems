                                                

#include "AbstractTimedActor.h"
#include <stdio.h>
#include <string.h>

AbstractTimedActor::AbstractTimedActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif








		, int stateSize, int hashmapSize
	) : 
	AbstractActor(myID, myName, maxQueueLength, maxParamLength, rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, out
		#endif








		, stateSize, hashmapSize
	) {
    deadline = new TIME_TYPE[maxQueueLength];
	memset (deadline, 0, sizeof(TIME_TYPE) * maxQueueLength);
    executionTime  = new TIME_TYPE[maxQueueLength];
	memset (executionTime,  0, sizeof(TIME_TYPE) * maxQueueLength);
	_ref_now = 0;
    #ifdef TTS
        __pc = -1;
        __res = 0;
    #endif

}

void AbstractTimedActor::preMessageExecutionInitialization() {

	_ref_currentMessageArrival = executionTime[0];
	_ref_currentMessageDeadline = deadline[0];
	_ref_currentMessageWaitingTime = _ref_now - _ref_currentMessageArrival;
	
}

AbstractTimedActor::~AbstractTimedActor() {

}

void AbstractTimedActor::setNow(TIME_TYPE now) {
    this->_ref_now = now;
}

TIME_TYPE AbstractTimedActor::getNow() {
    return this->_ref_now;
}

void AbstractTimedActor::addTimedBundles(byte senderId, TIME_TYPE executionTime, TIME_TYPE deadline) {
    timeEnqueue(this->executionTime, executionTime);
    timeEnqueue(this->deadline, deadline);
    applyPolicy(senderId == myID);
}

void AbstractTimedActor::timeEnqueue(TIME_TYPE *queue, TIME_TYPE value) {
	queue[queueTail] = value;
}

void AbstractTimedActor::applyPolicy(boolean isSendToSelf) {
	int cnt;
	for (cnt = queueTail; cnt > 0; cnt--) {
		if (executionTime[cnt] < executionTime[cnt - 1] && !(cnt == 1 && isSendToSelf)) {
			typedSwap(executionTime[cnt], executionTime[cnt - 1], TIME_TYPE)
			typedSwap(deadline[cnt], deadline[cnt - 1], TIME_TYPE)
			typedSwap(messageQueue[cnt], messageQueue[cnt - 1], byte)
			typedSwap(senderQueue[cnt], senderQueue[cnt - 1], byte)
			typedSwap(paramQueue[cnt], paramQueue[cnt - 1], byte*)
		} else {
			break;
		}
	}
	
	if (cnt > 0) {
		while ((cnt > 0) && (executionTime[cnt] == executionTime[cnt - 1]) && !(cnt == 1 && (isSendToSelf
		#ifdef TTS
			|| __pc != -1 
		#endif
		))) {
			if (messagePriorities[messageQueue[cnt]] > messagePriorities[messageQueue[cnt - 1]]) break;
			if (messagePriorities[messageQueue[cnt]] == messagePriorities[messageQueue[cnt - 1]]) {
				if (messageQueue[cnt] > messageQueue[cnt - 1]) break;
				if (messageQueue[cnt] == messageQueue[cnt - 1]) {
					if (senderQueue[cnt] > senderQueue[cnt - 1]) break;
					if (senderQueue[cnt] == senderQueue[cnt - 1]) {
						if (deadline[cnt] > deadline[cnt - 1]) break;
						if (deadline[cnt] == deadline[cnt - 1]) {
							int memcmpResult = memcmp(paramQueue[cnt], paramQueue[cnt - 1], maxParamLength);
							if (memcmpResult >= 0) break;
						}					
					}
				}
			}
			
			typedSwap(executionTime[cnt], executionTime[cnt - 1], TIME_TYPE)
			typedSwap(deadline[cnt], deadline[cnt - 1], TIME_TYPE)
			typedSwap(messageQueue[cnt], messageQueue[cnt - 1], byte)
			typedSwap(senderQueue[cnt], senderQueue[cnt - 1], byte)
			typedSwap(paramQueue[cnt], paramQueue[cnt - 1], byte*)
			cnt--;
		}
	}
}


void AbstractTimedActor::exportStateInXML(ostream &out, string tab) {
	out << tab << "<rebec name=\"" << myName << "\">" << endl;
	exportStateVariablesInXML(out, tab + "\t");
	exportQueueContentInXML(out, tab + "\t");	
	out << tab << '\t' << "<now>" << _ref_now << "</now>" << endl;
	#ifdef TTS
		char buffer[10];
		sprintf(buffer, "%d", __pc);
		out << tab << '\t' << "<pc>" << (__pc == -1 ? "" : buffer) << "</pc>" << endl;
		sprintf(buffer, "%d", __res);
		out << tab << '\t' << "<res>" << (__pc == -1 ? "" : buffer) << "</res>" << endl;
	#endif
	out << tab << "</rebec>" << endl;
}

void AbstractTimedActor::exportQueueContentInXML(ostream &out, string tab) {
	out << tab << "<queue>" << endl;
    for (int cnt = (shift ? 1 : 0); cnt < maxQueueLength && messageQueue[cnt] != 0; cnt++) {
        out << tab << '\t' << "<message arrival=\"" << (int)executionTime[cnt] << "\" deadline=\"";
        if (deadline[cnt] == MAX_TIME)
        	out << "infinity";
        else
        	out << (int) deadline[cnt];
        out << "\" sender=\"" << rebecs[senderQueue[cnt]]->getName()<< "\">";
        exportAMessageInXML(out, tab, cnt);
        out << "</message>" << endl;

    }        
	out << tab << "</queue>" << endl;	
}

void AbstractTimedActor::startTimer(Timer &t) {
	t = 0;
}
void AbstractTimedActor::stopTimer(Timer &t) {
	t = -1;
}
TIME_TYPE AbstractTimedActor::getTimerValue(Timer &t) {
	return t;
}










	TimedRebecStateIndexType AbstractTimedActor::marshalActor(TIME_TYPE& shiftTime) {
		long stateNo = marshalActor();
		
	    return storeLocalState(stateNo, shiftTime);
	}
	
	void AbstractTimedActor::unmarshalActor(TimedRebecStateIndexType state, TIME_TYPE shiftTime) {
	
	    TIME_TYPE bundleIndex = state.bundleIndex;
	    long stateNo = state.tableIndex;
	    vector<TimeFrame>** hashtableTimeExtension = getHashtableTimeExtension();
	    
	    unmarshalActor(stateNo);
	
	    TimeFrame frame = (hashtableTimeExtension[stateNo])->operator[](bundleIndex);
	    memcpy(executionTime, frame.executionTime, maxQueueLength * sizeof(TIME_TYPE));
	    memcpy(deadline, frame.deadline, maxQueueLength * sizeof(TIME_TYPE));
	    for (int cnt = 0; cnt < maxQueueLength && messageQueue[cnt]; cnt++) {
	        executionTime[cnt] += shiftTime;
	        if (deadline[cnt] != MAX_TIME)
	        	deadline[cnt] += shiftTime;
	    }
	    _ref_now = frame.now + shiftTime;
	    #ifdef TTS
			__res = (__pc == -1 ? 0 : (frame.res + shiftTime));
		#endif
	
	}
	
	TimedRebecStateIndexType AbstractTimedActor::storeLocalState(long stateNum, TIME_TYPE& shiftTime) {
		int cnt = 0;
	    vector<TimeFrame> **hashtableTimeExtension = getHashtableTimeExtension();
	    // Empty Queue equals to itself by any time shift operation
	    if (!hashtableTimeExtension[stateNum]) {
	    	hashtableTimeExtension[stateNum] = new vector<TimeFrame>();
	    }
	    if (!messageQueue[0] && !hashtableTimeExtension[stateNum]->empty()) {
	        TimedRebecStateIndexType retValue;
	        shiftTime = _ref_now - (*(hashtableTimeExtension[stateNum]->begin())).now;
	        retValue.tableIndex = stateNum;
	        retValue.bundleIndex = 0;
	        return retValue;
	    }
		for (vector<TimeFrame>::iterator it = hashtableTimeExtension[stateNum]->begin();
				it != hashtableTimeExtension[stateNum]->end(); it++, cnt++) {
	        TIME_TYPE diff = _ref_now - (*it).now;
	        bool mismatch = false;
	        #ifdef TTS
	        	if (__res != 0)
		        	mismatch = (diff != (__res - (*it).res));
			#endif
	        for (int cnt2 = shift; cnt2 < maxQueueLength && !mismatch && messageQueue[cnt2]; cnt2++) {
	            if ((diff != executionTime[cnt2] - (*it).executionTime[cnt2 - shift]) || 
	            	((diff != deadline[cnt2] - (*it).deadline[cnt2 - shift]) && deadline[cnt2] != MAX_TIME)) {
	                mismatch = true;
	                break;
	            }
	        }
	        if (!mismatch) {
	            TimedRebecStateIndexType retValue;
	            shiftTime = diff;
	            retValue.tableIndex = stateNum;
	            retValue.bundleIndex = cnt;
	            return retValue;
	        }
		}
	
	    timeBundleCount++;
	    TimeFrame newFrame;
	    newFrame.executionTime = new TIME_TYPE[maxQueueLength];
	    newFrame.deadline = new TIME_TYPE[maxQueueLength];
	    memcpy(newFrame.executionTime, &executionTime[shift], (maxQueueLength - shift) * sizeof (TIME_TYPE));
	    memcpy(newFrame.deadline, &deadline[shift], (maxQueueLength - shift)  * sizeof (TIME_TYPE));
	    if (shift) {
	    	newFrame.executionTime[maxQueueLength - 1] = 0;
	    	newFrame.deadline[maxQueueLength - 1] = MAX_TIME;
	    }
	    newFrame.now = _ref_now;
	    #ifdef TTS
	    	newFrame.res = __res;
		#endif
	    hashtableTimeExtension[stateNum]->push_back(newFrame);
	    TimedRebecStateIndexType retValue;
	    shiftTime = 0;
	    retValue.tableIndex = stateNum;
	    retValue.bundleIndex = cnt;
	    return retValue;
	}
		
