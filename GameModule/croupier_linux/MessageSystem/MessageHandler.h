#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

#include <iostream>
#include <memory>
#include <stdexcept>

#include "Message.h"

template<typename MTE>
class MessageSerializerBase {
public:
  virtual ~MessageSerializerBase() {}
  virtual MTE getMessageType() const = 0;
  virtual std::shared_ptr<MessageBase<MTE>> read(std::istream&) const = 0;
  virtual void write(std::ostream&, const std::shared_ptr<MessageBase<MTE>>&) const = 0;
};

class MessageSerializerImpl {
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
class MessageSerializer : public MessageSerializerBase<typename MessageClass::MessageType> {
public:
  typename MessageClass::MessageType getMessageType() const override {
    return MessageClass::Type;
  }

  std::shared_ptr<MessageBase<typename MessageClass::MessageType>> read(std::istream& is) const override {
    return MessageSerializerImpl::read<MessageClass>(is);
  }

  void write(std::ostream& os, const std::shared_ptr<MessageBase<typename MessageClass::MessageType>>& message) const override {
    auto typedMessage = std::static_pointer_cast<MessageClass>(message);
    MessageSerializerImpl::write<MessageClass>(os, typedMessage);
  }
};

template<typename MTE>
class MessageHandlerBase {
public:
  virtual ~MessageHandlerBase() {}
  virtual MTE getMessageType() const = 0;
  virtual void handle(std::shared_ptr<MessageBase<MTE>> message) = 0;
};

template<typename HandlerClass, typename MessageClass>
class MessageHandler : public MessageHandlerBase<typename MessageClass::MessageType> {
public:
  typedef void(HandlerClass::*FunctionType)(const typename MessageClass::TupleType&);

private:
  HandlerClass& handler;
  FunctionType function;

public:
  MessageHandler(HandlerClass& handler, FunctionType function) : handler(handler), function(function) {}

  typename MessageClass::MessageType getMessageType() const override {
    return MessageClass::Type;
  }

  void handle(std::shared_ptr<MessageBase<typename MessageClass::MessageType>> message) override {
    auto typedMessage = std::dynamic_pointer_cast<MessageClass>(message);
    (handler.*function)(typedMessage->getTuple());
  }
};

#endif /* MESSAGEHANDLER_H_ */
