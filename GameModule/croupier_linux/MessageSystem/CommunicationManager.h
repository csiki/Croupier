#ifndef COMMUNICATIONMANAGER_H_
#define COMMUNICATIONMANAGER_H_

#include <map>
#include <memory>

#include "Message.h"
#include "MessageHandler.h"

template<typename MTE>
class CommunicationManager {
  std::map<MTE, std::shared_ptr<MessageHandlerBase<MTE>>> messageHandlers;

public:
  void registerHandler(std::shared_ptr<MessageHandlerBase<MTE>> handler) {
    messageHandlers[handler->getMessageType()] = handler;
  }

  void processIncomingMessage(std::shared_ptr<MessageBase<MTE>> message) {
    messageHandlers.at(message->getType())->handle(message);
  }

  void readMessage(std::istream& is) {
    unsigned type;
    is >> type;
    auto x = messageHandlers.at(static_cast<MTE>(type))->read(is);
    processIncomingMessage(x);
  }

  template<typename MessageClass>
  void writeMessage(std::ostream& os, const std::shared_ptr<MessageClass>& message) {
    messageHandlers.at(message->getType())->write(os, message);
  }
};

#endif /* COMMUNICATIONMANAGER_H_ */
