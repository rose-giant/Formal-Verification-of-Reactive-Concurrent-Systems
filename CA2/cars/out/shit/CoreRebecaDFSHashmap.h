                                    

#ifndef __CORE_REBECA_DFS_HASHMAP_H_
#define __CORE_REBECA_DFS_HASHMAP_H_

#include "Types.h"
#include "Config.h"
#include <ostream>

using namespace std;

/**
Suppose we have n processes, we need
	2 flags if we use NDFS only
	n+2 flags if we use fairness (extended state space) plus NDFS
	if we use partial order reduction, we need two more flags
		one for marking on-stack (used only in first DFS)
		another to indicate if current state had been fully expanded in fisrt search
		The second flag is also used in the first DFS to dinstinguish between thw two phases of the loop over processes
*/

struct State {

	long rebecs[REBEC_COUNT];
	short int claim;
    long stateID;
    State *next;	// in bucket

 // n + 3
    byte
	    flag1;

	boolean isOnStack();
	void clearOnStack();
	void setOnStack();
	boolean isVisited(byte counter);
	void visit(short int counter);	// visit this state in State-Space #counter
	boolean equals (long *sys, long claim);
};

class CoreRebecaDFSHashmap {
	State** table;
	int hashmapSize;
	long hashValue;
	//State* current;
	unsigned int hashCode (void*, unsigned long, unsigned int);
	ostream &out;
public:
	CoreRebecaDFSHashmap (int hashmapSize, ostream &out);
	~CoreRebecaDFSHashmap ();
	State* exists (long* sys, long claim);
	State* put (long*, long);
	//State* getState();
};

#endif
