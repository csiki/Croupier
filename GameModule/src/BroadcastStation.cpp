#include "stdafx.h"
#include "BroadcastStation.h"
#include "BroadcastMember.h"

/** Sends a message for all subscribers except the sender.
*/
void BroadcastStation::broadcast(BroadcastMember* sender, BroadcastMessage msg, int dataSize, const int* data)
{
	int senderID = sender->getID();

	for (std::list<BroadcastMember*>::iterator it = this->receivers.begin(); it != this->receivers.end(); ++it)
	{
		if (*it != sender)
		{
			(*it)->receiveBroadcast(senderID, msg, dataSize, data);
		}
	}
}

/** Subscribes to station.
*/
void BroadcastStation::subsrcibe(BroadcastMember* newMember)
{
	for (std::list<BroadcastMember*>::iterator it = this->receivers.begin(); it != this->receivers.end(); ++it)
	{
		if (*it == newMember)
		{
			return; // already subscribed
		}
	}

	// subscribe
	this->receivers.push_back(newMember);
}

/** Unsubscribe a member.
*/
void BroadcastStation::unsubsrcibe(BroadcastMember* member)
{
	for (std::list<BroadcastMember*>::iterator it = this->receivers.begin(); it != this->receivers.end(); ++it)
	{
		if (*it == member)
		{
			this->receivers.erase(it); // unsubscribe
			return;
		}
	}
}