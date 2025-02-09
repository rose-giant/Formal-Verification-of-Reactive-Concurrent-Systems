












#ifndef __COMMUNICATIONDEVICE_ACTOR_H__
#define __COMMUNICATIONDEVICE_ACTOR_H__

class CPUActor;
class SensorActor;
class MiscActor;
class WirelessMediumActor;

#include "AbstractTimedActor.h"

class CommunicationDeviceActor :
	public virtual AbstractTimedActor
{

public:
    CommunicationDeviceActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~CommunicationDeviceActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	byte _ref_id;

    	int _ref_sendingData;

    	int _ref_sendingPacketsNumber;

    	CommunicationDeviceActor* _ref_receiverDevice;


		long constructorCommunicationDevice(byte creatorID
	        ,
    		byte
	         _ref_myId
		);

       virtual void _msg_send(byte senderId
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
       virtual long msgsrvsend(
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
       virtual void _msg_receiveStatus(byte senderId
	        ,
    		boolean
	         _ref_result
        )
        ;
       virtual long msgsrvreceiveStatus(
    		boolean
	         _ref_result
        )
        ;
       virtual void _msg_receiveResult(byte senderId
	        ,
    		boolean
	         _ref_result
        )
        ;
       virtual long msgsrvreceiveResult(
    		boolean
	         _ref_result
        )
        ;
       virtual void _msg_receiveData(byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_receivingPacketsNumber
        )
        ;
       virtual long msgsrvreceiveData(
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_receivingPacketsNumber
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_send(byte senderId
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
      	  void _timed_msg_receiveStatus(byte senderId
	        ,
    		boolean
	         _ref_result
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_receiveResult(byte senderId
	        ,
    		boolean
	         _ref_result
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_receiveData(byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_receivingPacketsNumber
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif