#if !defined(_BROADCASTSTATION_H)
#define _BROADCASTSTATION_H

#include "BroadcastMember.h"
#include "BroadcastMessage.h"

class BroadcastStation
{
private:
	BroadcastMember *receivers;
public:
	void broadcast(BroadcastMember* member, BroadcastMessage msg, int dataSize, const int* const data);
	void subsrcibe(BroadcastMember* newMember);
	void unsubsrcibe(BroadcastMember* member);
};

#endif  //_BROADCASTSTATION_H
