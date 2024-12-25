












#ifndef __INFRASTRUCTURE_ACTOR_H__
#define __INFRASTRUCTURE_ACTOR_H__

class ControllerActor;
class AutoActor;
class MannedActor;

#include "AbstractActor.h"

class InfrastructureActor :
	public virtual AbstractActor
{

public:
    InfrastructureActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
    );
	~InfrastructureActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	std::array<AutoActor*, 10> _ref_autoCars;

    	std::array<MannedActor*, 10> _ref_mannedCars;


		long constructorInfrastructure(byte creatorID
		);

       virtual void _msg_isAllowed(byte senderId
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
        )
        ;
       virtual long msgsrvisAllowed(
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
        )
        ;
       virtual void _msg_canMove(byte senderId
	        ,
    		boolean
	         _ref_canMove
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
        )
        ;
       virtual long msgsrvcanMove(
    		boolean
	         _ref_canMove
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();
protected:
};

#endif