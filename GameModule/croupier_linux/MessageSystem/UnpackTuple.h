#ifndef UNPACKTUPLE_H_
#define UNPACKTUPLE_H_

#include <functional>
#include <tuple>

class UnpackTuple {
private:
  template<int...>
  class Sequence {};

  template<int N, int... S>
  struct GenerateSequence : GenerateSequence<N-1, N-1, S...> {};

  template<int... S> struct GenerateSequence<0, S...> {
    typedef Sequence<S...> type;
  };

  template<typename Handler, typename Fun, typename Args, int... S>
  static void callMemberFunctionImpl(Handler& handler, Fun function, const Args& args, Sequence<S...>) {
    (handler.*function)(std::get<S>(args)...);
  }

public:
  template<typename Handler, typename Fun, typename Args>
  static void callMemberFunction(Handler& handler, Fun function, const Args& args) {
    typedef typename GenerateSequence<std::tuple_size<Args>::value>::type genSeq;
    callMemberFunctionImpl(handler, function, args, genSeq());
  }

  template<typename ReturnType, typename HandlerClass, typename ParameterValues>
  struct GetMemberFunctionType {};

  template<typename ReturnType, typename HandlerClass, typename... T>
  struct GetMemberFunctionType<ReturnType, HandlerClass, std::tuple<T...>> {
    typedef ReturnType(HandlerClass::*type)(T...);
  };

};

#endif /* UNPACKTUPLE_H_ */
