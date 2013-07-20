#include "stdafx.h"
#include "BroadcastStation.h"
#include "BroadcastMember.h"

/** Sends a message for all other members.
*/
void BroadcastMember::broadcast(BroadcastMessage msg, int dataSize, const int* data)
{
	this->broadcastStation->broadcast(this, msg, dataSize, data);
}

/** Subscribe to BroadcastStation.
*/
void BroadcastMember::subscribe()
{
	this->broadcastStation->subsrcibe(this);
}

/** Unsubscribe from BroadcastStation.
*/
void BroadcastMember::unsubscribe()
{
	this->broadcastStation->unsubsrcibe(this);
}

