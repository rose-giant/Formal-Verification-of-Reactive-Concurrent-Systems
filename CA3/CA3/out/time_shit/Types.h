

#ifndef __MODERE_TYPES_H__
#define __MODERE_TYPES_H__

#include <exception>
#include <iostream>
#include <string>
 
using namespace std;

#define INITIAL_SENDER 99

#ifndef null
#define null 0
#endif

#ifndef boolean
#define boolean bool
#endif

#ifndef ArrayList
#define ArrayList vector
#endif


#ifndef byte
//Modified by Ehsan (byte is a signed value, why MMJ changed it to unsigned?)
#define byte char
#endif


#define REBEC_COUNT 4

class AbstractActor;
class Exception: public exception {
protected:
	AbstractActor *causedBy;
	string message;
public:
	Exception(string message) {
		this->message = message;
		this->causedBy = null;
	}
	Exception(string message, AbstractActor* causedBy) {
		this->message = message;
		this->causedBy = causedBy;
	}
	~Exception() throw() {}
	const string getMessage() {
		return message;
	}
	AbstractActor* getCausedBy() const {
		return causedBy;
	}
	void setCausedBy(AbstractActor* causedBy) {
		this->causedBy = causedBy;
	}
};

class AssertionFailedException: public Exception {
public:
	AssertionFailedException(string message) : Exception(message){}
	AssertionFailedException(string message, AbstractActor* causedBy) : Exception(message, causedBy){} 
	~AssertionFailedException() throw() {}
};

class QueueOverflowException: public Exception {
public:
	QueueOverflowException(string message) : Exception(message){}
	QueueOverflowException(string message, AbstractActor* causedBy) : Exception(message, causedBy){} 
	~QueueOverflowException() throw() {}
};

class HeapOverflowException: public Exception {
public:
	HeapOverflowException(string message) : Exception(message){}
	~HeapOverflowException() throw() {}
};

class DeadlockException: public Exception {
public:
	DeadlockException(string message) : Exception(message){}
	DeadlockException(string message, AbstractActor* causedBy) : Exception(message, causedBy){} 
	~DeadlockException() throw() {}
};

class CounterExampleException : public Exception {
public:
	CounterExampleException(string message) : Exception(message){}
	~CounterExampleException() throw() {}
};

class UnknownPropertyException : public Exception {
public:
	UnknownPropertyException(string message) : Exception(message){}
	~UnknownPropertyException() throw() {}
};

 /*
  * The hash function is taken from SPIN, and is based on
  * the public domain hash function from Bob Jenkins, 1996.
  * see: http://www.burtleburtle.net/bob/
  * It is changed to fit the data structures.
  */
#define mix(a,b,c) \
{ a -= b; a -= c; a ^= (c >> 13); \
  b -= c; b -= a; b ^= (a << 8);  \
  c -= a; c -= b; c ^= (b >> 13); \
  a -= b; a -= c; a ^= (c >> 12); \
  b -= c; b -= a; b ^= (a << 16); \
  c -= a; c -= b; c ^= (b >> 5);  \
  a -= b; a -= c; a ^= (c >> 3);  \
  b -= c; b -= a; b ^= (a << 10); \
  c -= a; c -= b; c ^= (b >> 15); \
}

#define NEW_STATE 1

#define randint(x) (rand() % (x + 1))

extern int BOOLEAN_SIZE, BYTE_SIZE, SHORT_SIZE, INT_SIZE, FLOAT_SIZE, DOUBLE_SIZE, REACTIVE_CLASS_SIZE;

string operator+(const string left, const int right);
string operator+(const string left, const short right);
string operator+(const string left, const byte right);
string operator+(const string left, const float right);
string operator+(const string left, const double right);
string operator+(const string left, const boolean right);
class AbstractActor;
string operator+(const string left, const AbstractActor *right);

#endif