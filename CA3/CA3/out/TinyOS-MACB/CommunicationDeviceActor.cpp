                                    


#include "Types.h"
#include "CommunicationDeviceActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "CPUActor.h"
#include "SensorActor.h"
#include "MiscActor.h"
#include "WirelessMediumActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define SEND 1
#define RECEIVESTATUS 2
#define RECEIVERESULT 3
#define RECEIVEDATA 4


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<CommunicationDeviceActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_medium (dynamic_cast<WirelessMediumActor*>(rebecs[(int)((unsigned char)known[1])]))
    
CommunicationDeviceActor::CommunicationDeviceActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[4 + 1];
    messagePriorities = new int[4 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"SEND";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"RECEIVESTATUS";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"RECEIVERESULT";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"RECEIVEDATA";
			messagePriorities[4] = std::numeric_limits<int>::max();








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

CommunicationDeviceActor::~CommunicationDeviceActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void CommunicationDeviceActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case SEND: {

            	CommunicationDeviceActor* _ref_receiver;
            
            
                	memcpy(&_ref_receiver, &paramQueue[cnt] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);

            	int _ref_data;
            
            
                	memcpy(&_ref_data, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_packetsNumber;
            
            
                	memcpy(&_ref_packetsNumber, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "send("
                	<< (_ref_receiver == NULL ? "NULL" : _ref_receiver->getName())
					<< ", "
                	<< ((int)_ref_data)
					<< ", "
                	<< ((int)_ref_packetsNumber)
                << ")";
                break;
               }
            case RECEIVESTATUS: {

            	boolean _ref_result;
            
            
_ref_result = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "receiveStatus("
                	<< (_ref_result? "true" : "false")
                << ")";
                break;
               }
            case RECEIVERESULT: {

            	boolean _ref_result;
            
            
_ref_result = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "receiveResult("
                	<< (_ref_result? "true" : "false")
                << ")";
                break;
               }
            case RECEIVEDATA: {

            	CommunicationDeviceActor* _ref_receiver;
            
            
                	memcpy(&_ref_receiver, &paramQueue[cnt] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);

            	int _ref_data;
            
            
                	memcpy(&_ref_data, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_receivingPacketsNumber;
            
            
                	memcpy(&_ref_receivingPacketsNumber, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "receiveData("
                	<< (_ref_receiver == NULL ? "NULL" : _ref_receiver->getName())
					<< ", "
                	<< ((int)_ref_data)
					<< ", "
                	<< ((int)_ref_receivingPacketsNumber)
                << ")";
                break;
               }
    	}
}


void CommunicationDeviceActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void CommunicationDeviceActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"CommunicationDevice.id\" type=\"byte\">" <<  
			    		((int)_ref_id) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"CommunicationDevice.sendingData\" type=\"int\">" <<  
			    		((int)_ref_sendingData) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"CommunicationDevice.sendingPacketsNumber\" type=\"int\">" <<  
			    		((int)_ref_sendingPacketsNumber) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"CommunicationDevice.receiverDevice\" type=\"CommunicationDevice\">" <<  
			    		(_ref_receiverDevice == NULL ? "NULL" : _ref_receiverDevice->getName()) << "</variable>" << endl;
}


char* CommunicationDeviceActor::getClassName() {
    return (char*)"CommunicationDevice";
}
void CommunicationDeviceActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * BYTE_SIZE));
	                marshalIndex += (1 * BYTE_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_sendingData, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_sendingPacketsNumber, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_receiverDevice, (1 * REACTIVE_CLASS_SIZE));
	                marshalIndex += (1 * REACTIVE_CLASS_SIZE);

    
	
}
void CommunicationDeviceActor::marshalActorToArray(byte* array) {
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

void CommunicationDeviceActor::unmarshalActorFromArray(byte* array) {

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

void CommunicationDeviceActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_id, &array [index], (1 * BYTE_SIZE));
	                index += (1 * BYTE_SIZE);

            
            
                	memcpy(&_ref_sendingData, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_sendingPacketsNumber, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_receiverDevice, &array [index], (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);
    
	
}

/*#ifdef DEBUG
void CommunicationDeviceActor::printStateContent(long stateNo) {
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
long CommunicationDeviceActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case SEND:
                {
                    int i = 0;

            	CommunicationDeviceActor* _ref_receiver;
            
            
                	memcpy(&_ref_receiver, &paramQueue[0] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);

            	int _ref_data;
            
            
                	memcpy(&_ref_data, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_packetsNumber;
            
            
                	memcpy(&_ref_packetsNumber, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvsend(
            _ref_receiver
	        , 
            _ref_data
	        , 
            _ref_packetsNumber
                    );
                }
            case RECEIVESTATUS:
                {
                    int i = 0;

            	boolean _ref_result;
            
            
_ref_result = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvreceiveStatus(
            _ref_result
                    );
                }
            case RECEIVERESULT:
                {
                    int i = 0;

            	boolean _ref_result;
            
            
_ref_result = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvreceiveResult(
            _ref_result
                    );
                }
            case RECEIVEDATA:
                {
                    int i = 0;

            	CommunicationDeviceActor* _ref_receiver;
            
            
                	memcpy(&_ref_receiver, &paramQueue[0] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);

            	int _ref_data;
            
            
                	memcpy(&_ref_data, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_receivingPacketsNumber;
            
            
                	memcpy(&_ref_receivingPacketsNumber, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvreceiveData(
            _ref_receiver
	        , 
            _ref_data
	        , 
            _ref_receivingPacketsNumber
                    );
                }
    }
	return -1;
}


    long CommunicationDeviceActor::constructorCommunicationDevice (byte creatorId 
	        ,
    		byte
	         _ref_myId
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "CommunicationDevice";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref_myId);
			(_ref_sendingData=0);
			(_ref_sendingPacketsNumber=0);
			(_ref_receiverDevice=null);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void CommunicationDeviceActor::_msg_send (byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SEND);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_receiver, (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_data, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_packetsNumber, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationDeviceActor::_msg_receiveStatus (byte senderId
	        ,
    		boolean
	         _ref_result
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RECEIVESTATUS);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_result ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationDeviceActor::_msg_receiveResult (byte senderId
	        ,
    		boolean
	         _ref_result
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RECEIVERESULT);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_result ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationDeviceActor::_msg_receiveData (byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_receivingPacketsNumber
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RECEIVEDATA);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_receiver, (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_data, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_receivingPacketsNumber, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** CommunicationDeviceActor::table = (byte**)0xFFFFFFFF;

	byte** CommunicationDeviceActor::getTable() {
		return table;
	}








	vector<TimeFrame>** CommunicationDeviceActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** CommunicationDeviceActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void CommunicationDeviceActor::_timed_msg_send (byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_send (senderId
	        ,
            _ref_receiver
	        , 
            _ref_data
	        , 
            _ref_packetsNumber
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationDeviceActor::_timed_msg_receiveStatus (byte senderId
	        ,
    		boolean
	         _ref_result
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_receiveStatus (senderId
	        ,
            _ref_result
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationDeviceActor::_timed_msg_receiveResult (byte senderId
	        ,
    		boolean
	         _ref_result
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_receiveResult (senderId
	        ,
            _ref_result
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationDeviceActor::_timed_msg_receiveData (byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_receivingPacketsNumber
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_receiveData (senderId
	        ,
            _ref_receiver
	        , 
            _ref_data
	        , 
            _ref_receivingPacketsNumber
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CommunicationDeviceActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long CommunicationDeviceActor::msgsrvsend (
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
    ) {
    			long arrayIndexChecker = 0;WirelessMediumActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "send";
		#endif

	#ifdef TTS
	#endif

		{
			assertion((_ref_receiverDevice==null));
			(_ref_sendingPacketsNumber=_ref_packetsNumber);
			(_ref_receiverDevice=_ref_receiver);
			(_ref_sendingData=_ref_data);
			(temp0=_ref_medium, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 179"), temp0)->_timed_msg_getStatus(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long CommunicationDeviceActor::msgsrvreceiveStatus (
    		boolean
	         _ref_result
    ) {
    			long arrayIndexChecker = 0;WirelessMediumActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "receiveStatus";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
		if (__pc == 1) {__pc = -1; goto __jumpPoint1;}
	#endif

		{
			assertion((!_ref_result));
				__res = _ref_now + ((_ref_numberOfNodes/2)*(_ref_OnePacketTransmissionTime+1));
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
			(temp0=_ref_medium, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 185"), temp0)->_timed_msg_broadcast(myID, _ref_receiverDevice, _ref_sendingData, _ref_sendingPacketsNumber, _ref_now, MAX_TIME);
				__res = _ref_now + (_ref_OnePacketTransmissionTime*_ref_sendingPacketsNumber);
	__pc = 1;
	shift = 0;
	return 0;
	__jumpPoint1:
;
		}
		return 0;

}
	    long CommunicationDeviceActor::msgsrvreceiveResult (
    		boolean
	         _ref_result
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "receiveResult";
		#endif

	#ifdef TTS
	#endif

		{
			assertion(_ref_result);
			(_ref_sendingPacketsNumber=0);
			(_ref_receiverDevice=null);
			(_ref_sendingData=0);
		}
		return 0;

}
	    long CommunicationDeviceActor::msgsrvreceiveData (
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_receivingPacketsNumber
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "receiveData";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
			if ((_ref_receiver==_ref_self)) {
				{
						__res = _ref_now + (_ref_receivingPacketsNumber*_ref_OnePacketTransmissionTime);
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
				}
}
		}
		return 0;

}
