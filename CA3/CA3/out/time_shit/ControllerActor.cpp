                                    


#include "Types.h"
#include "ControllerActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "InfrastructureActor.h"
#include "AutoActor.h"
#include "MannedActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define ISALLOWED 1


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<ControllerActor*>(rebecs[(int)((unsigned char)myID)]))

    
ControllerActor::ControllerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[1 + 1];
    messagePriorities = new int[1 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"ISALLOWED";
			messagePriorities[1] = std::numeric_limits<int>::max();








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

ControllerActor::~ControllerActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void ControllerActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case ISALLOWED: {

            	boolean _ref_line;
            
            

            	int _ref_block;
            
            
                	memcpy(&_ref_block, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_id;
            
            
                	memcpy(&_ref_id, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	boolean _ref_isAuto;
            
            
_ref_line = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[1]);
++i;                out << "isAllowed("
                	<< (_ref_line? "true" : "false")
					<< ", "
                	<< ((int)_ref_block)
					<< ", "
                	<< ((int)_ref_id)
					<< ", "
                	<< (_ref_isAuto? "true" : "false")
                << ")";
                break;
               }
    	}
}


void ControllerActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void ControllerActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Controller.shouldStop\" type=\"boolean[10]\">" <<  
			    		"[" << (_ref_shouldStop[0]? "true" : "false") << ", " << (_ref_shouldStop[1]? "true" : "false") << ", " << (_ref_shouldStop[2]? "true" : "false") << ", " << (_ref_shouldStop[3]? "true" : "false") << ", " << (_ref_shouldStop[4]? "true" : "false") << ", " << (_ref_shouldStop[5]? "true" : "false") << ", " << (_ref_shouldStop[6]? "true" : "false") << ", " << (_ref_shouldStop[7]? "true" : "false") << ", " << (_ref_shouldStop[8]? "true" : "false") << ", " << (_ref_shouldStop[9]? "true" : "false") << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.shouldChangeLine\" type=\"boolean[10]\">" <<  
			    		"[" << (_ref_shouldChangeLine[0]? "true" : "false") << ", " << (_ref_shouldChangeLine[1]? "true" : "false") << ", " << (_ref_shouldChangeLine[2]? "true" : "false") << ", " << (_ref_shouldChangeLine[3]? "true" : "false") << ", " << (_ref_shouldChangeLine[4]? "true" : "false") << ", " << (_ref_shouldChangeLine[5]? "true" : "false") << ", " << (_ref_shouldChangeLine[6]? "true" : "false") << ", " << (_ref_shouldChangeLine[7]? "true" : "false") << ", " << (_ref_shouldChangeLine[8]? "true" : "false") << ", " << (_ref_shouldChangeLine[9]? "true" : "false") << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.autoLine\" type=\"boolean[10]\">" <<  
			    		"[" << (_ref_autoLine[0]? "true" : "false") << ", " << (_ref_autoLine[1]? "true" : "false") << ", " << (_ref_autoLine[2]? "true" : "false") << ", " << (_ref_autoLine[3]? "true" : "false") << ", " << (_ref_autoLine[4]? "true" : "false") << ", " << (_ref_autoLine[5]? "true" : "false") << ", " << (_ref_autoLine[6]? "true" : "false") << ", " << (_ref_autoLine[7]? "true" : "false") << ", " << (_ref_autoLine[8]? "true" : "false") << ", " << (_ref_autoLine[9]? "true" : "false") << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.mannedLine\" type=\"boolean[10]\">" <<  
			    		"[" << (_ref_mannedLine[0]? "true" : "false") << ", " << (_ref_mannedLine[1]? "true" : "false") << ", " << (_ref_mannedLine[2]? "true" : "false") << ", " << (_ref_mannedLine[3]? "true" : "false") << ", " << (_ref_mannedLine[4]? "true" : "false") << ", " << (_ref_mannedLine[5]? "true" : "false") << ", " << (_ref_mannedLine[6]? "true" : "false") << ", " << (_ref_mannedLine[7]? "true" : "false") << ", " << (_ref_mannedLine[8]? "true" : "false") << ", " << (_ref_mannedLine[9]? "true" : "false") << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.answeredAuto\" type=\"boolean\">" <<  
			    		(_ref_answeredAuto? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.answeredManned\" type=\"boolean\">" <<  
			    		(_ref_answeredManned? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.autoBlock\" type=\"int[10]\">" <<  
			    		"[" << ((int)_ref_autoBlock[0]) << ", " << ((int)_ref_autoBlock[1]) << ", " << ((int)_ref_autoBlock[2]) << ", " << ((int)_ref_autoBlock[3]) << ", " << ((int)_ref_autoBlock[4]) << ", " << ((int)_ref_autoBlock[5]) << ", " << ((int)_ref_autoBlock[6]) << ", " << ((int)_ref_autoBlock[7]) << ", " << ((int)_ref_autoBlock[8]) << ", " << ((int)_ref_autoBlock[9]) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.mannedBlock\" type=\"int[10]\">" <<  
			    		"[" << ((int)_ref_mannedBlock[0]) << ", " << ((int)_ref_mannedBlock[1]) << ", " << ((int)_ref_mannedBlock[2]) << ", " << ((int)_ref_mannedBlock[3]) << ", " << ((int)_ref_mannedBlock[4]) << ", " << ((int)_ref_mannedBlock[5]) << ", " << ((int)_ref_mannedBlock[6]) << ", " << ((int)_ref_mannedBlock[7]) << ", " << ((int)_ref_mannedBlock[8]) << ", " << ((int)_ref_mannedBlock[9]) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.lastWasAuto\" type=\"boolean\">" <<  
			    		(_ref_lastWasAuto? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Controller.safetyCheck\" type=\"boolean\">" <<  
			    		(_ref_safetyCheck? "true" : "false") << "</variable>" << endl;
}


char* ControllerActor::getClassName() {
    return (char*)"Controller";
}
void ControllerActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
            
            
            
            
            
            
					memcpy(&array [marshalIndex], _ref_autoBlock.data(), (1 * INT_SIZE) * 10);					
					marshalIndex += (1 * INT_SIZE) * 10;
            
					memcpy(&array [marshalIndex], _ref_mannedBlock.data(), (1 * INT_SIZE) * 10);					
					marshalIndex += (1 * INT_SIZE) * 10;
            
            

array [marshalIndex] = 0;
array [marshalIndex] |= ((_ref_shouldStop)[0] ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= ((_ref_shouldStop)[1] ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= ((_ref_shouldStop)[2] ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= ((_ref_shouldStop)[3] ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= ((_ref_shouldStop)[4] ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= ((_ref_shouldStop)[5] ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= ((_ref_shouldStop)[6] ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
array [marshalIndex] |= ((_ref_shouldStop)[7] ? 0xFF : 0x00) & AbstractActor::coeff[7]; 
marshalIndex++;
array [marshalIndex] = 0;
array [marshalIndex] |= ((_ref_shouldStop)[8] ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= ((_ref_shouldStop)[9] ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[0] ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[1] ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[2] ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[3] ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[4] ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[5] ? 0xFF : 0x00) & AbstractActor::coeff[7]; 
marshalIndex++;
array [marshalIndex] = 0;
array [marshalIndex] |= ((_ref_shouldChangeLine)[6] ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[7] ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[8] ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= ((_ref_shouldChangeLine)[9] ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= ((_ref_autoLine)[0] ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= ((_ref_autoLine)[1] ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= ((_ref_autoLine)[2] ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
array [marshalIndex] |= ((_ref_autoLine)[3] ? 0xFF : 0x00) & AbstractActor::coeff[7]; 
marshalIndex++;
array [marshalIndex] = 0;
array [marshalIndex] |= ((_ref_autoLine)[4] ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= ((_ref_autoLine)[5] ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= ((_ref_autoLine)[6] ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= ((_ref_autoLine)[7] ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= ((_ref_autoLine)[8] ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= ((_ref_autoLine)[9] ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= ((_ref_mannedLine)[0] ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
array [marshalIndex] |= ((_ref_mannedLine)[1] ? 0xFF : 0x00) & AbstractActor::coeff[7]; 
marshalIndex++;
array [marshalIndex] = 0;
array [marshalIndex] |= ((_ref_mannedLine)[2] ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= ((_ref_mannedLine)[3] ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= ((_ref_mannedLine)[4] ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= ((_ref_mannedLine)[5] ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= ((_ref_mannedLine)[6] ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= ((_ref_mannedLine)[7] ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= ((_ref_mannedLine)[8] ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
array [marshalIndex] |= ((_ref_mannedLine)[9] ? 0xFF : 0x00) & AbstractActor::coeff[7]; 
marshalIndex++;
array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_answeredAuto ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= (_ref_answeredManned ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= (_ref_lastWasAuto ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= (_ref_safetyCheck ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
++marshalIndex;    
	
}
void ControllerActor::marshalActorToArray(byte* array) {
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

void ControllerActor::unmarshalActorFromArray(byte* array) {

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

void ControllerActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            

            
            

            
            

            
            

            
            

            
            

            
            
					memcpy(_ref_autoBlock.data(), &array [index], (1 * INT_SIZE) * 10);					
					index += (1 * INT_SIZE) * 10;

            
            
					memcpy(_ref_mannedBlock.data(), &array [index], (1 * INT_SIZE) * 10);					
					index += (1 * INT_SIZE) * 10;

            
            

            
            
_ref_shouldStop[0] = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_shouldStop[1] = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_shouldStop[2] = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_shouldStop[3] = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_shouldStop[4] = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_shouldStop[5] = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_shouldStop[6] = (bool) (array [index] & AbstractActor::coeff[6]);
_ref_shouldStop[7] = (bool) (array [index] & AbstractActor::coeff[7]);
index++;
_ref_shouldStop[8] = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_shouldStop[9] = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_shouldChangeLine[0] = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_shouldChangeLine[1] = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_shouldChangeLine[2] = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_shouldChangeLine[3] = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_shouldChangeLine[4] = (bool) (array [index] & AbstractActor::coeff[6]);
_ref_shouldChangeLine[5] = (bool) (array [index] & AbstractActor::coeff[7]);
index++;
_ref_shouldChangeLine[6] = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_shouldChangeLine[7] = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_shouldChangeLine[8] = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_shouldChangeLine[9] = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_autoLine[0] = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_autoLine[1] = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_autoLine[2] = (bool) (array [index] & AbstractActor::coeff[6]);
_ref_autoLine[3] = (bool) (array [index] & AbstractActor::coeff[7]);
index++;
_ref_autoLine[4] = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_autoLine[5] = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_autoLine[6] = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_autoLine[7] = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_autoLine[8] = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_autoLine[9] = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_mannedLine[0] = (bool) (array [index] & AbstractActor::coeff[6]);
_ref_mannedLine[1] = (bool) (array [index] & AbstractActor::coeff[7]);
index++;
_ref_mannedLine[2] = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_mannedLine[3] = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_mannedLine[4] = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_mannedLine[5] = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_mannedLine[6] = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_mannedLine[7] = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_mannedLine[8] = (bool) (array [index] & AbstractActor::coeff[6]);
_ref_mannedLine[9] = (bool) (array [index] & AbstractActor::coeff[7]);
index++;
_ref_answeredAuto = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_answeredManned = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_lastWasAuto = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_safetyCheck = (bool) (array [index] & AbstractActor::coeff[3]);
++index;    
	
}

/*#ifdef DEBUG
void ControllerActor::printStateContent(long stateNo) {
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
long ControllerActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case ISALLOWED:
                {
                    int i = 0;

            	boolean _ref_line;
            
            

            	int _ref_block;
            
            
                	memcpy(&_ref_block, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_id;
            
            
                	memcpy(&_ref_id, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	boolean _ref_isAuto;
            
            
_ref_line = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[0] [i] & AbstractActor::coeff[1]);
++i;                    return msgsrvisAllowed(
            _ref_line
	        , 
            _ref_block
	        , 
            _ref_id
	        , 
            _ref_isAuto
                    );
                }
    }
	return -1;
}


    long ControllerActor::constructorController (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Controller";
		#endif

	#ifdef TTS
	#endif

		{
			_synchmethod_initVars();
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}

 
		void ControllerActor::_synchmethod_initVars (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "initVars";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_safetyCheck=false);
			(_ref_lastWasAuto=false);
			for (int _ref_i = 0;
((_ref_i<_ref_MAX_BLOCK_NUM)); (_ref_i++)) {
				{
					(_ref_shouldStop[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 35") , arrayIndexChecker)]=false);
					(_ref_shouldChangeLine[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 36") , arrayIndexChecker)]=false);
					(_ref_autoLine[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 37") , arrayIndexChecker)]=true);
					(_ref_mannedLine[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 38") , arrayIndexChecker)]=false);
					(_ref_autoBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 39") , arrayIndexChecker)]=100);
					(_ref_mannedBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 40") , arrayIndexChecker)]=200);
				}
}
		}

		}
 
		void ControllerActor::_synchmethod_check (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "check";
		#endif

	#ifdef TTS
	#endif

		{
			for (int _ref_i = 0;
((_ref_i<_ref_MAX_BLOCK_NUM)); (_ref_i++)) {
				{
					for (int _ref_j = 0;
((_ref_j<_ref_MAX_BLOCK_NUM)); (_ref_j++)) {
						{
							if (((_ref_autoLine[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 50") , arrayIndexChecker)]!=_ref_mannedLine[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 50") , arrayIndexChecker)])&&((_ref_autoBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 50") , arrayIndexChecker)]==(_ref_mannedBlock[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 50") , arrayIndexChecker)]+1))||(_ref_autoBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 50") , arrayIndexChecker)]==(_ref_mannedBlock[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 50") , arrayIndexChecker)]+1))))) {
								{
									(_ref_shouldStop[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 51") , arrayIndexChecker)]=true);
								}
}
							if (((_ref_autoLine[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 54") , arrayIndexChecker)]==_ref_mannedLine[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 54") , arrayIndexChecker)])&&((_ref_autoBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 54") , arrayIndexChecker)]==(_ref_mannedBlock[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 54") , arrayIndexChecker)]+1))||(_ref_autoBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 54") , arrayIndexChecker)]==(_ref_mannedBlock[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 54") , arrayIndexChecker)]+1))))) {
								{
									(_ref_shouldStop[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 55") , arrayIndexChecker)]=true);
									(_ref_shouldChangeLine[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 56") , arrayIndexChecker)]=true);
								}
}
							if (((_ref_autoLine[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 59") , arrayIndexChecker)]==_ref_mannedLine[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 59") , arrayIndexChecker)])&&(_ref_autoBlock[(arrayIndexChecker=_ref_i, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 59") , arrayIndexChecker)]==_ref_mannedBlock[(arrayIndexChecker=_ref_j, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 59") , arrayIndexChecker)]))) {
								{
									(_ref_safetyCheck=true);
								}
}
							assertion((!_ref_safetyCheck));
						}
}
				}
}
		}

		}


		void ControllerActor::_msg_isAllowed (byte senderId
	        ,
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
  	  ){
        queueTail = 0;
        enqueue(messageQueue, ISALLOWED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_block, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_id, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_line ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
paramsValues [index] |= (_ref_isAuto ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** ControllerActor::table = (byte**)0xFFFFFFFF;

	byte** ControllerActor::getTable() {
		return table;
	}








	vector<TimeFrame>** ControllerActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** ControllerActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void ControllerActor::_timed_msg_isAllowed (byte senderId
	        ,
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_isAllowed (senderId
	        ,
            _ref_line
	        , 
            _ref_block
	        , 
            _ref_id
	        , 
            _ref_isAuto
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void ControllerActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long ControllerActor::msgsrvisAllowed (
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
    ) {
    			long arrayIndexChecker = 0;InfrastructureActor *temp0;InfrastructureActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "isAllowed";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
				__res = _ref_now + 1;
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
			if (_ref_isAuto) {
				{
					(_ref_autoBlock[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 73") , arrayIndexChecker)]=_ref_block);
					(_ref_autoLine[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 74") , arrayIndexChecker)]=_ref_line);
					_synchmethod_check();
					(temp0=(dynamic_cast<InfrastructureActor*>(_ref_sender)), assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 77"), temp0)->_timed_msg_canMove(myID, _ref_shouldStop[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 77") , arrayIndexChecker)], _ref_id, true, _ref_now, MAX_TIME);
					(_ref_lastWasAuto=true);
				}
}
			if ((!_ref_isAuto)) {
				{
					(_ref_mannedBlock[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 82") , arrayIndexChecker)]=_ref_block);
					(_ref_mannedLine[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 83") , arrayIndexChecker)]=_ref_line);
					_synchmethod_check();
					(temp1=(dynamic_cast<InfrastructureActor*>(_ref_sender)), assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 86"), temp1)->_timed_msg_canMove(myID, _ref_shouldChangeLine[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 86") , arrayIndexChecker)], _ref_id, false, _ref_now, MAX_TIME);
					(_ref_lastWasAuto=false);
				}
}
		}
		return 0;

}
