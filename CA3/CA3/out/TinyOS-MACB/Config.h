

#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "Types.h"

#define SAFE_MODE
#define PROGRESS_REPORT
#define EXPORT_STATE_SPACE


#if (defined DEBUG || defined EXPORT_STATE_SPACE)
	#define ENABLE_STATE_ID
#endif

	 extern int _ref_samplingRate;
 
	 extern int _ref_numberOfNodes;
 
	 extern int _ref_bufferSize;
 
	 extern int _ref_sensorTaskDelay;
 
	 extern int _ref_tmdaSlotSize;
 
	 extern int _ref_miscPeriod;
 
	 extern int _ref_OnePacketTransmissionTime;
 
	 extern int _ref_miscTaskDelay;
 
	 extern int _ref_packetMaximumSize;
 


#define NEW_STATE 1



#define TTS

#ifdef TTS
#define NO_ATOMIC_EXECUTION
#endif
#define TIME_TYPE int
#define MAX_TIME 0x7FFFFFFF
#define Timer TIME_TYPE

class AbstractActor;
class DeadlineMissedException: public Exception {
public:
	DeadlineMissedException(string message) : Exception(message) {}
	DeadlineMissedException(string message, AbstractActor* causedBy) : Exception(message, causedBy){} 
	~DeadlineMissedException() throw() {}
};


#endif