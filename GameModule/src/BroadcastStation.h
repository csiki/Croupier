#if !defined(_BROADCASTSTATION_H)
#define _BROADCASTSTATION_H

#include <list>
#include "BroadcastMember.h"
#include "BroadcastMessage.h"

/**	Spreades the messages amongst subscribed broadcast members (observer pattern).
*/
class BroadcastStation
{
private:
	//list<int> receivers;
public:
	void broadcast(BroadcastMember* member, BroadcastMessage msg, int dataSize, const int* data);
	void subsrcibe(BroadcastMember* newMember);
	void unsubsrcibe(BroadcastMember* member);
};

#endif  //_BROADCASTSTATION_H
