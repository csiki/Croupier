#ifndef NOTIFIABLEI_HPP
#define NOTIFIABLEI_HPP

/*
	This includes the interfaces without any implementation code.
*/


// Predefined NotDataI IDs here (values<0)
#define NOT_EVT_ALIVE       -2  // An alive signal, do nothing (also allows the source to count number of subscribers)
#define NOT_EVT_DESTRUCTOR	-1  // Destructor notification
#define NOT_EVT_ALL         0   // All notifications (on subscribing)


class NotifierI;   //  (source)
class NotifiableI; //  (target)

// Notifications has one ID and one NotDataI* pdata


/** @ingroup VObj 
 * This is the base class for data passed along when notifying. */
class NotDataI {
public:
	/** For async notification, make a copy and store for later delivery. */ 
	virtual NotDataI* docall Clone(){ return NULL; };
};


/** @ingroup VObj 
 * The receiver (target) of notifications.
 * The expectation is that these methods are thread safe. */
class NotifiableI {
public:
	/** This function gets called when the source has a notification. */ 
	virtual bool docall OnNotify(NotifierI* src, int evt, NotDataI *pnd=NULL ) = 0;
};

/** @ingroup VObj 
 * The generator (source) of notifications. 
 * The expectation is that these methods are thread safe. */
class NotifierI {
public:
	/** Add a notification target. 
     * @param pnot is the target, where we send notifications. 
     * @param evt is the type of notifications we want to know about, set to 
     * 0 to receive all notifications. */ 
	virtual bool docall AddNotifiable(NotifiableI* pnot, int evt) = 0;
	/** Remove a notification target.
     * @param pnot is the target, that should have no more notifications. 
     * @param evt is the type of notifications we stop subscribing to, set to 
     * 0 to stop for all notifications. */ 
	virtual bool docall RemoveNotifiable(NotifiableI* pnot, int evt) = 0;
	/** Trigger send notification to all added targets. 
     * @param evt is the event type to send 
     * @param pnd is an optional structure with data
     * @return Returns the number of objects that were notified */
	virtual int docall Notify(int evt, NotDataI *pnd) = 0;
};

#endif // NOTIFIABLEI_HPP

