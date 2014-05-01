#ifndef COMMUNICATIONMANAGER_H_
#define COMMUNICATIONMANAGER_H_

#include <atomic>
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>

#include "Message.h"
#include "MessageHandler.h"

template<typename MessageClass>
class ResponseHandler : public MessageHandlerBase<typename MessageClass::MessageType> {
private:
  std::function<void(std::shared_ptr<MessageClass>)> handler;

public:
  ResponseHandler(const std::function<void(std::shared_ptr<MessageClass>)>& handler) : handler(handler) {}

  typename MessageClass::MessageType getMessageType() const override {
    return MessageClass::Type;
  }

  void handle(std::shared_ptr<MessageBase<typename MessageClass::MessageType>> message) {
    auto typedMessage = std::dynamic_pointer_cast<MessageClass>(message);
    handler(typedMessage);
  }
};

template<typename MTE>
class CommunicationManager {
  typedef std::shared_ptr<MessageHandlerBase<MTE>> MessageHandlerPtr;
  typedef std::shared_ptr<MessageSerializerBase<MTE>> MessageSerializerPtr;

  std::istream& is;
  std::ostream& os;

  std::map<MTE, MessageSerializerPtr> messageSerializers;
  std::map<MTE, MessageHandlerPtr> messageHandlers;

  std::mutex messageSerializers_guard;
  std::mutex messageHandlers_guard;

  std::atomic_bool runReader;
  std::thread reader;

  MessageHandlerPtr getMessageHandler(MTE messageType) {
    std::lock_guard<std::mutex> lock(messageHandlers_guard);
    auto it = messageHandlers.find(messageType);
    if(it != messageHandlers.end()) {
      return it->second;
    } else {
      return nullptr;
    }
  }

  MessageSerializerPtr getMessageSerializer(MTE messageType) {
    std::lock_guard<std::mutex> lock(messageSerializers_guard);
    auto it = messageSerializers.find(messageType);
    if(it != messageSerializers.end()) {
      return it->second;
    } else {
      return nullptr;
    }
  }

  void handleMessage(std::shared_ptr<MessageBase<MTE>> message) {
    auto handler = getMessageHandler(message->getType());
    if(handler != nullptr) {
      handler->handle(message);
    }
  }

  void readMessage() {
    const size_t BufferSize = 4096;
    const char MessageSeparator = '\n';
    const unsigned RetryTimeout = 1; // ms

    char buffer[BufferSize];
    std::stringstream ss;

    while(runReader.load()) {
      if(size_t len = is.readsome(buffer, BufferSize)) {
        size_t start = 0;
        for(size_t i = 0; i < len; ++i) {
          if(buffer[i] == MessageSeparator) {
            ss.write(&buffer[start], i - start + 1);
            start = i + 1;

            unsigned type; ss >> type;
            auto serializer = getMessageSerializer(static_cast<MTE>(type));
            if(serializer != nullptr) {
              auto message = serializer->read(ss);
              if(ss.good()) {
                handleMessage(message);
              }
            }
            ss.clear();
            ss.str("");
          }
        }
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(RetryTimeout));
      }
    }
  }

public:
  CommunicationManager(std::istream& is, std::ostream& os) : is(is), os(os), runReader(true),
    reader(&CommunicationManager::readMessage, this) {
  }

  ~CommunicationManager() {
    runReader.store(false);
    reader.join();
  }

  void registerSerializer(std::shared_ptr<MessageSerializerBase<MTE>> serializer) {
    std::lock_guard<std::mutex> lock(messageSerializers_guard);
    messageSerializers[serializer->getMessageType()] = serializer;
  }

  void registerHandler(std::shared_ptr<MessageHandlerBase<MTE>> handler) {
    std::lock_guard<std::mutex> lock(messageHandlers_guard);
    messageHandlers[handler->getMessageType()] = handler;
  }

  template<typename MessageClass>
  void sendMessage(std::shared_ptr<MessageClass> message) {
    std::lock_guard<std::mutex> lock(messageSerializers_guard);
    messageSerializers.at(message->getType())->write(os, message);
  }
};

#endif /* COMMUNICATIONMANAGER_H_ */
