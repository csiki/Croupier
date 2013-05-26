#if !defined(_BROADCASTSTATION_H)
#define _BROADCASTSTATION_H

#include "BroadcastMessage.h"

class BroadcastMember; // cross reference

class BroadcastStation
{
private:
	BroadcastMember **receivers;
public:
	void broadcast(BroadcastMember* member, BroadcastMessage msg, int dataSize, const int* data);
	void subsrcibe(BroadcastMember* newMember);
	void unsubsrcibe(BroadcastMember* member);
};

#endif  //_BROADCASTSTATION_H
