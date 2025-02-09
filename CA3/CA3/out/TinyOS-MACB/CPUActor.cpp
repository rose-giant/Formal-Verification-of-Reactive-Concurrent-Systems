                                    


#include "Types.h"
#include "CPUActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "SensorActor.h"
#include "MiscActor.h"
#include "WirelessMediumActor.h"
#include "CommunicationDeviceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define SENSOREVENT 1
#define SENSORTASK 2
#define MISCEVENT 3


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<CPUActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_senderDevice (dynamic_cast<CommunicationDeviceActor*>(rebecs[(int)((unsigned char)known[1])]))
    	#define _ref_receiverDevice (dynamic_cast<CommunicationDeviceActor*>(rebecs[(int)((unsigned char)known[2])]))
    	#define _ref_sensor (dynamic_cast<SensorActor*>(rebecs[(int)((unsigned char)known[3])]))
    
CPUActor::CPUActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














)
	:
		AbstractTimedActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize














		)
	, AbstractActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, out
		#endif







		, stateSize, hashmapSize














	)
{
    messageNames = new char *[3 + 1];
    messagePriorities = new int[3 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"SENSOREVENT";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"SENSORTASK";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"MISCEVENT";
			messagePriorities[3] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
	}








	if (hashtableTimeExtension == (vector<TimeFrame>**)0xFFFFFFFF) {	// avoid reinitialization
        hashtableTimeExtension = new vector<TimeFrame>*[1 << hashmapSize];
        memset (hashtableTimeExtension, 0, sizeof(vector<TimeFrame>*)*(1 << hashmapSize));
	}







	
	
	queueTail = 0;
}

CPUActor::~CPUActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void CPUActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case SENSOREVENT: {

            	int _ref_period;
            
            
                	memcpy(&_ref_period, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "sensorEvent("
                	<< ((int)_ref_period)
                << ")";
                break;
               }
            case SENSORTASK: {

            	int _ref_period;
            
            
                	memcpy(&_ref_period, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_lag;
            
            
                	memcpy(&_ref_lag, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "sensorTask("
                	<< ((int)_ref_period)
					<< ", "
                	<< ((int)_ref_lag)
                << ")";
                break;
               }
            case MISCEVENT: {
                out << "miscEvent("
                << ")";
                break;
               }
    	}
}


void CPUActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void CPUActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"CPU.collectedSamplesCounter\" type=\"int\">" <<  
			    		((int)_ref_collectedSamplesCounter) << "</variable>" << endl;
}


char* CPUActor::getClassName() {
    return (char*)"CPU";
}
void CPUActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_collectedSamplesCounter, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void CPUActor::marshalActorToArray(byte* array) {
    int i,j, marshalIndex = 0;

	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = messageQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }
    
	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = senderQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }

	if(maxParamLength != 0) {
        for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = paramQueue[i][j];
            }
        }
        if (shift) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = 0;
            }
        }
    }

	 #ifdef NO_ATOMIC_EXECUTION
    	memcpy(&array [marshalIndex], &__pc, 4);
    	marshalIndex += 4;
    	//array[marshalIndex++] = __pc;
    #endif

		marshalVariablesToArray( & array[marshalIndex]);

   

}

void CPUActor::unmarshalActorFromArray(byte* array) {

    int index = 0, i, j;
    shift = 0;

	for (i = 0; i < maxQueueLength; i++) {
        messageQueue[i] = array[index++];
    }
	
	for (i = 0; i < maxQueueLength; i++) {
        senderQueue[i] = array[index++];
    }
    
	if(maxParamLength != 0) {
        for (i = 0; i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                paramQueue[i][j] = array[index++];
            }
        }
    }
    
	#ifdef NO_ATOMIC_EXECUTION
    	memcpy(&__pc, &array [index], 4);
    	index += 4;
    	//__pc = array[index++];
    #endif
    
	unmarshalVariablesFromArray( & array[index]);
}

void CPUActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_collectedSamplesCounter, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void CPUActor::printStateContent(long stateNo) {
	out << myName << "=>[";
	byte* currentValues = getTable()[stateNo];
	for (int cnt = 0; cnt < STATE_SIZE; cnt++)
		out << (int)currentValues[cnt] << ", ";
	out << "]";
}
#endif
*/

//---------------------------------------------------------
/**
  Checks which action of this rebec is enabled, and executes it. Actions are the
  same as the location-counter defined at the beginning of this file.
  In the case of non-atomic message servers, a location-counter may point to the
  middle of a message server.
  If this action leads to more than one transition (non_det), this method executes
  only one of these transitions. With the help of some static variables, consecutive
  calls to this method execute different transitions caused by this action.
  Local variables must be properly valuated before any call to this method (see marshal()).

  returns: >0 : there are more possible next states (non-det transitions).
		   =0 : no more non_det transitions
		   -1 : no enabled action (empty queue)
		   -2 : Local hash table is full - immediate termination of search
 */
long CPUActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case SENSOREVENT:
                {
                    int i = 0;

            	int _ref_period;
            
            
                	memcpy(&_ref_period, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvsensorEvent(
            _ref_period
                    );
                }
            case SENSORTASK:
                {
                    int i = 0;

            	int _ref_period;
            
            
                	memcpy(&_ref_period, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_lag;
            
            
                	memcpy(&_ref_lag, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvsensorTask(
            _ref_period
	        , 
            _ref_lag
                    );
                }
            case MISCEVENT:
                {
                    int i = 0;
                    return msgsrvmiscEvent(
                    );
                }
    }
	return -1;
}


    long CPUActor::constructorCPU (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "CPU";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_collectedSamplesCounter=0);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void CPUActor::_msg_sensorEvent (byte senderId
	        ,
    		int
	         _ref_period
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SENSOREVENT);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_period, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CPUActor::_msg_sensorTask (byte senderId
	        ,
    		int
	         _ref_period
	        , 
    		int
	         _ref_lag
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SENSORTASK);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_period, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_lag, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CPUActor::_msg_miscEvent (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, MISCEVENT);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** CPUActor::table = (byte**)0xFFFFFFFF;

	byte** CPUActor::getTable() {
		return table;
	}








	vector<TimeFrame>** CPUActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** CPUActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void CPUActor::_timed_msg_sensorEvent (byte senderId
	        ,
    		int
	         _ref_period
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_sensorEvent (senderId
	        ,
            _ref_period
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CPUActor::_timed_msg_sensorTask (byte senderId
	        ,
    		int
	         _ref_period
	        , 
    		int
	         _ref_lag
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_sensorTask (senderId
	        ,
            _ref_period
	        , 
            _ref_lag
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CPUActor::_timed_msg_miscEvent (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_miscEvent (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CPUActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long CPUActor::msgsrvsensorEvent (
    		int
	         _ref_period
    ) {
    			long arrayIndexChecker = 0;CPUActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "sensorEvent";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 44"), temp0)->_timed_msg_sensorTask(myID, _ref_period, _ref_currentMessageWaitingTime, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long CPUActor::msgsrvsensorTask (
    		int
	         _ref_period
	        , 
    		int
	         _ref_lag
    ) {
    			long arrayIndexChecker = 0;CommunicationDeviceActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "sensorTask";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
			assertion((((_ref_period-_ref_lag)-_ref_currentMessageWaitingTime)>=0));
				__res = _ref_now + _ref_sensorTaskDelay;
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
			(_ref_collectedSamplesCounter+=1);
			if ((_ref_collectedSamplesCounter==_ref_bufferSize)) {
				{
					(temp0=_ref_senderDevice, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 61"), temp0)->_timed_msg_send(myID, _ref_receiverDevice, 0, 1, _ref_now, MAX_TIME);
					(_ref_collectedSamplesCounter=0);
				}
}
		}
		return 0;

}
	    long CPUActor::msgsrvmiscEvent (
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "miscEvent";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
				__res = _ref_now + _ref_miscTaskDelay;
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
		}
		return 0;

}
