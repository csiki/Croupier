#include "stdafx.h"
#include "BroadcastStation.h"
#include "BroadcastMember.h"

void BroadcastMember::broadcast(BroadcastMessage msg, int dataSize, const int* data)
{
	this->broadcastStation->broadcast(this, msg, dataSize, data);
}

void BroadcastMember::subscribe()
{
	this->broadcastStation->subsrcibe(this);
}

void BroadcastMember::unsubscribe()
{
	this->broadcastStation->unsubsrcibe(this);
}

