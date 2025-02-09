                                    


#include "Types.h"
#include "SensorActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "CPUActor.h"
#include "MiscActor.h"
#include "WirelessMediumActor.h"
#include "CommunicationDeviceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define SENSORFIRST 1
#define SENSORLOOP 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<SensorActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_cpu (dynamic_cast<CPUActor*>(rebecs[(int)((unsigned char)known[1])]))
    
SensorActor::SensorActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[2 + 1];
    messagePriorities = new int[2 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"SENSORFIRST";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"SENSORLOOP";
			messagePriorities[2] = std::numeric_limits<int>::max();








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

SensorActor::~SensorActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void SensorActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case SENSORFIRST: {
                out << "sensorFirst("
                << ")";
                break;
               }
            case SENSORLOOP: {
                out << "sensorLoop("
                << ")";
                break;
               }
    	}
}


void SensorActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void SensorActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


}


char* SensorActor::getClassName() {
    return (char*)"Sensor";
}
void SensorActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
	
}
void SensorActor::marshalActorToArray(byte* array) {
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

void SensorActor::unmarshalActorFromArray(byte* array) {

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

void SensorActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
	
}

/*#ifdef DEBUG
void SensorActor::printStateContent(long stateNo) {
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
long SensorActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case SENSORFIRST:
                {
                    int i = 0;
                    return msgsrvsensorFirst(
                    );
                }
            case SENSORLOOP:
                {
                    int i = 0;
                    return msgsrvsensorLoop(
                    );
                }
    }
	return -1;
}


    long SensorActor::constructorSensor (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;SensorActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Sensor";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 78"), temp0)->_timed_msg_sensorFirst(myID, _ref_now, MAX_TIME);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void SensorActor::_msg_sensorFirst (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SENSORFIRST);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void SensorActor::_msg_sensorLoop (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SENSORLOOP);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** SensorActor::table = (byte**)0xFFFFFFFF;

	byte** SensorActor::getTable() {
		return table;
	}








	vector<TimeFrame>** SensorActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** SensorActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void SensorActor::_timed_msg_sensorFirst (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_sensorFirst (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void SensorActor::_timed_msg_sensorLoop (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_sensorLoop (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void SensorActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long SensorActor::msgsrvsensorFirst (
    ) {
    			long arrayIndexChecker = 0;SensorActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "sensorFirst";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 10;

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 82"), temp0)->_timed_msg_sensorLoop(myID, (__tNumber|=1, nonDetVariable0) + _ref_now, MAX_TIME);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 10) {
				nonDetVariable0 = 20;
			}
			else if(nonDetVariable0 == 20) {
				nonDetVariable0 = 30;
			}
			else if(nonDetVariable0 == 30) {
				nonDetVariable0 = 10;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long SensorActor::msgsrvsensorLoop (
    ) {
    			long arrayIndexChecker = 0;CPUActor *temp0;SensorActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "sensorLoop";
		#endif

	#ifdef TTS
	#endif

		{
			int _ref_period = (1000/_ref_samplingRate);

			(temp0=_ref_cpu, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 87"), temp0)->_timed_msg_sensorEvent(myID, _ref_period, _ref_now, MAX_TIME);
			(temp1=_ref_self, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 88"), temp1)->_timed_msg_sensorLoop(myID, _ref_period + _ref_now, MAX_TIME);
		}
		return 0;

}
