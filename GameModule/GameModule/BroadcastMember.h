#if !defined(_BROADCASTMEMBER_H)
#define _BROADCASTMEMBER_H

#include "Entity.h"
#include "BroadcastStation.h"
#include "BroadcastMessage.h"

class BroadcastMember : public Entity
{
private:
	BroadcastStation *broadcastStation;
protected:
	void broadcast(BroadcastMessage msg, int dataSize, const int* data);
	void subscribe();
	void unsubscribe();
public:
	virtual void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data) = 0;
};

#endif  //_BROADCASTMEMBER_H
