












#ifndef __WIRELESSMEDIUM_ACTOR_H__
#define __WIRELESSMEDIUM_ACTOR_H__

class CPUActor;
class SensorActor;
class MiscActor;
class CommunicationDeviceActor;

#include "AbstractTimedActor.h"

class WirelessMediumActor :
	public virtual AbstractTimedActor
{

public:
    WirelessMediumActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~WirelessMediumActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	CommunicationDeviceActor* _ref_senderDevice;

    	CommunicationDeviceActor* _ref_receiverDevice;

    	int _ref_maxTraffic;


		long constructorWirelessMedium(byte creatorID
		);

       virtual void _msg_getStatus(byte senderId
        )
        ;
       virtual long msgsrvgetStatus(
        )
        ;
       virtual void _msg_broadcast(byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
        )
        ;
       virtual long msgsrvbroadcast(
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
        )
        ;
       virtual void _msg_broadcastingIsCompleted(byte senderId
        )
        ;
       virtual long msgsrvbroadcastingIsCompleted(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_getStatus(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_broadcast(byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_broadcastingIsCompleted(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif