












#ifndef __MISC_ACTOR_H__
#define __MISC_ACTOR_H__

class CPUActor;
class SensorActor;
class WirelessMediumActor;
class CommunicationDeviceActor;

#include "AbstractTimedActor.h"

class MiscActor :
	public virtual AbstractTimedActor
{

public:
    MiscActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~MiscActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorMisc(byte creatorID
		);

       virtual void _msg_miscFirst(byte senderId
        )
        ;
       virtual long msgsrvmiscFirst(
        )
        ;
       virtual void _msg_miscLoop(byte senderId
        )
        ;
       virtual long msgsrvmiscLoop(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_miscFirst(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_miscLoop(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif