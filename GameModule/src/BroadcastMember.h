#if !defined(_BROADCASTMEMBER_H)
#define _BROADCASTMEMBER_H

#include "Entity.h"
#include "BroadcastMessage.h"

class BroadcastStation; // cross reference

/**	Base class for entities communicating through BroadcastStation (observer pattern).
*/
class BroadcastMember : virtual public Entity
{
private:
	BroadcastStation *broadcastStation;
public: // TODO protected
	void broadcast(BroadcastMessage msg, int dataSize, const int* data);
	void subscribe();
	void unsubscribe();
public:

	BroadcastMember(int id, BroadcastStation* broadcastStation) : Entity(id)
	{
		this->broadcastStation = broadcastStation;
	}

	virtual void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data) = 0;
};

#endif  //_BROADCASTMEMBER_H
