












#ifndef __CPU_ACTOR_H__
#define __CPU_ACTOR_H__

class SensorActor;
class MiscActor;
class WirelessMediumActor;
class CommunicationDeviceActor;

#include "AbstractTimedActor.h"

class CPUActor :
	public virtual AbstractTimedActor
{

public:
    CPUActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~CPUActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_collectedSamplesCounter;


		long constructorCPU(byte creatorID
		);

       virtual void _msg_sensorEvent(byte senderId
	        ,
    		int
	         _ref_period
        )
        ;
       virtual long msgsrvsensorEvent(
    		int
	         _ref_period
        )
        ;
       virtual void _msg_sensorTask(byte senderId
	        ,
    		int
	         _ref_period
	        , 
    		int
	         _ref_lag
        )
        ;
       virtual long msgsrvsensorTask(
    		int
	         _ref_period
	        , 
    		int
	         _ref_lag
        )
        ;
       virtual void _msg_miscEvent(byte senderId
        )
        ;
       virtual long msgsrvmiscEvent(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_sensorEvent(byte senderId
	        ,
    		int
	         _ref_period
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_sensorTask(byte senderId
	        ,
    		int
	         _ref_period
	        , 
    		int
	         _ref_lag
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_miscEvent(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif