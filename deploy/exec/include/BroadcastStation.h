#if !defined(_BROADCASTSTATION_H)
#define _BROADCASTSTATION_H

#include "BroadcastMessage.h"

class BroadcastMember; // cross referencing

/**	Spreades the messages amongst subscribed broadcast members (observer pattern).
*/
class BroadcastStation
{
private:
	std::list<BroadcastMember*> receivers;
public:

	BroadcastStation() {}

	void broadcast(BroadcastMember* sender, BroadcastMessage msg, int dataSize, const int* data); // UNIT done
	void subsrcibe(BroadcastMember* newMember);
	void unsubsrcibe(BroadcastMember* member);
};

#endif  //_BROADCASTSTATION_H
