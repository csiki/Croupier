#include "stdafx.h"
#include "BroadcastStation.h"
#include "BroadcastMember.h"

/** Sends a message for all subscribers except the sender.
*/
void BroadcastStation::broadcast(BroadcastMember* member, BroadcastMessage msg, int dataSize, const int* data)
{
	int senderID = member->getID();

	for (list<BroadcastMember*>::iterator it = this->receivers.begin(); it != this->receivers.end(); ++it)
	{
		if ((*it)->getID() != senderID)
		{
			(*it)->receiveBroadcast(senderID, msg, dataSize, data);
		}
	}
}

/** Subscribes to station.
*/
void BroadcastStation::subsrcibe(BroadcastMember* newMember)
{
	int newMemberID = newMember->getID();

	for (list<BroadcastMember*>::iterator it = this->receivers.begin(); it != this->receivers.end(); ++it)
	{
		if ((*it)->getID() == newMemberID)
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
	int memberID = member->getID();

	for (list<BroadcastMember*>::iterator it = this->receivers.begin(); it != this->receivers.end(); ++it)
	{
		if ((*it)->getID() == memberID)
		{
			this->receivers.erase(it); // unsubscribe
			return;
		}
	}
}

