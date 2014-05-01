#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

#include <iostream>
#include <memory>
#include <stdexcept>

#include "Message.h"

template<typename MTE>
class MessageHandlerBase {
public:
  virtual ~MessageHandlerBase() {}
  virtual void handle(std::shared_ptr<MessageBase<MTE>> message) = 0;
  virtual MTE getMessageType() const = 0;
  virtual std::shared_ptr<MessageBase<MTE>> read(std::istream&) const = 0;
  virtual void write(std::ostream&, const std::shared_ptr<MessageBase<MTE>>&) const = 0;
};

class MessageSerializer {
private:
  template<typename MessageClass, unsigned I>
  struct readImpl {
    static void read(std::istream& is, std::shared_ptr<MessageClass>& res) {
      readImpl<MessageClass, I-1>::read(is, res);
      is >> res->template get<static_cast<typename MessageClass::FieldNames>(I-1)>();
    }
  };

  template<typename MessageClass>
  struct readImpl<MessageClass, 0> {
    static void read(std::istream&, std::shared_ptr<MessageClass>&) {}
  };

  template<typename MessageClass, unsigned I>
  struct writeImpl {
    static void write(std::ostream& os, const std::shared_ptr<MessageClass>& message) {
      writeImpl<MessageClass, I - 1>::write(os, message);
      os << ' ' << message->template get<static_cast<typename MessageClass::FieldNames>(I-1)>();
    }
  };

  template<typename MessageClass>
  struct writeImpl<MessageClass, 0> {
    static void write(std::ostream&, const std::shared_ptr<MessageClass>&) {}
  };

public:
  template<typename MessageClass>
  static void write(std::ostream& os, const std::shared_ptr<MessageClass>& message) {
    os << static_cast<unsigned>(MessageClass::Type);
    writeImpl<MessageClass, MessageClass::Size>::write(os, message);
    os << std::endl;
  }

  template<typename MessageClass>
  static std::shared_ptr<MessageClass> read(std::istream& is) {
    auto message = std::make_shared<MessageClass>();
    readImpl<MessageClass, MessageClass::Size>::read(is, message);
    return message;
  }
};

template<typename MessageClass>
class BasicMessageHandler : public MessageHandlerBase<typename MessageClass::MessageType> {
public:
  virtual ~BasicMessageHandler() {}

  virtual void handle(std::shared_ptr<MessageBase<typename MessageClass::MessageType>>) override {
    throw std::runtime_error("Not expected message type received!");
  }

  std::shared_ptr<MessageBase<typename MessageClass::MessageType>> read(std::istream& is) const override {
    return MessageSerializer::read<MessageClass>(is);
  }

  void write(std::ostream& os, const std::shared_ptr<MessageBase<typename MessageClass::MessageType>>& message) const override {
    auto typedMessage = std::static_pointer_cast<MessageClass>(message);
    MessageSerializer::write<MessageClass>(os, typedMessage);
  }

  typename MessageClass::MessageType getMessageType() const override {
    return MessageClass::Type;
  }
};

template<typename HandlerClass, typename MessageClass>
class MessageHandler : public BasicMessageHandler<MessageClass> {
public:
  typedef void(HandlerClass::*FunctionType)(const typename MessageClass::TupleType&);

private:
  std::shared_ptr<HandlerClass> handler;
  FunctionType function;

public:
  MessageHandler(std::shared_ptr<HandlerClass>& handler, FunctionType function) : handler(handler), function(function) {}

  void handle(std::shared_ptr<MessageBase<typename MessageClass::MessageType>> message) override {
    auto typesMessage = std::dynamic_pointer_cast<MessageClass>(message);
    ((*handler).*function)(typesMessage->getTuple());
  }
};

#endif /* MESSAGEHANDLER_H_ */
