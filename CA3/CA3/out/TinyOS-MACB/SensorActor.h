












#ifndef __SENSOR_ACTOR_H__
#define __SENSOR_ACTOR_H__

class CPUActor;
class MiscActor;
class WirelessMediumActor;
class CommunicationDeviceActor;

#include "AbstractTimedActor.h"

class SensorActor :
	public virtual AbstractTimedActor
{

public:
    SensorActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~SensorActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorSensor(byte creatorID
		);

       virtual void _msg_sensorFirst(byte senderId
        )
        ;
       virtual long msgsrvsensorFirst(
        )
        ;
       virtual void _msg_sensorLoop(byte senderId
        )
        ;
       virtual long msgsrvsensorLoop(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_sensorFirst(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_sensorLoop(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif