#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <tuple>

template<typename MTE>
class MessageBase {
public:
  virtual ~MessageBase() {}
  virtual MTE getType() const = 0;
};

template<typename MTE, MTE MT, typename FN, typename... FT>
class Message : public MessageBase<MTE> {
private:
  std::tuple<FT...> fields;

public:
  typedef FN FieldNames;
  typedef std::tuple<FT...> TupleType;
  typedef MTE MessageType;
  static const MTE Type = MT;
  static const size_t Size = std::tuple_size<std::tuple<FT...>>::value;

  Message() {}
  Message(FT... args) : fields(args...) {}

  template<FN field>
  auto get() -> decltype(std::get<static_cast<unsigned>(field)>(fields)) {
    return std::get<static_cast<unsigned>(field)>(fields);
  }

  template<FN field>
  auto get() const -> decltype(std::get<static_cast<unsigned>(field)>(fields)) {
    return std::get<static_cast<unsigned>(field)>(fields);
  }

  MTE getType() const override {
    return MT;
  }

  const TupleType& getTuple() const {
    return fields;
  }
};

#define EXPAND(...) __VA_ARGS__
#define CREATE_MESSAGE(type, name, fieldNames, fieldTypes) \
  enum class name ##_Fields { EXPAND fieldNames }; \
  typedef Message<type, type::name, name ##_Fields, EXPAND fieldTypes> name;

#endif /* MESSAGE_H_ */
